/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection_Methods.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:00:00 by alafon            #+#    #+#             */
/*   Updated: 2025/11/19 19:52:11 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

/*
 * Function: parseCgiResponse
 * Role: Split CGI output into headers and body, applying status overrides from the Status header.
 *
 * Parameters:
 *   output - Raw CGI output stream.
 *   response - HttpResponse object to populate with status and body.
 */
static void	parseCgiResponse(const std::string& output,
						HttpResponse& response)
{
	std::string				headerPart;
	std::string				bodyPart;
	const std::string		delimiter = "\r\n\r\n";
	std::string::size_type	pos = output.find(delimiter);

	if (pos == std::string::npos)
		pos = output.find("\n\n");

	if (pos != std::string::npos)
	{
		headerPart = output.substr(0, pos);
		bodyPart = output.substr(pos + delimiter.size());
	}
	else
		bodyPart = output;

	std::istringstream	headerStream(headerPart);
	std::string			line;
	int					statusCode = 0;
	std::string			statusText;

	while (std::getline(headerStream, line) && !line.empty())
	{
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		if (line.empty())
			break;
		std::string::size_type sep = line.find(':');
		if (sep == std::string::npos)
			continue;
		std::string name = line.substr(0, sep);
		std::string value = line.substr(sep + 1);
		while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
			value.erase(0, 1);
		if (name == "Status")
		{
			std::istringstream statusStream(value);
			if (statusStream >> statusCode)
			{
				std::getline(statusStream, statusText);
				while (!statusText.empty() && statusText[0] == ' ')
					statusText.erase(0, 1);
			}
		}
	}
	if (statusCode == 0)
	{
		statusCode = 200;
		statusText = "OK";
	}
	response.setStatusCode(statusCode);
	response.setStatusText(statusText);
	response.setBody(bodyPart);
}

/*==============================================================*/
/*                    HTTP METHOD HANDLERS                      */
/*==============================================================*/

/*
 * Function: ClientConnection::methodGet
 * Role: Serve GET or HEAD requests by resolving the resource, executing CGI when configured, or generating autoindex.
 *
 * Parameters:
 *   body - true to include body data (GET), false for HEAD responses.
 *
 * Returns:
 *   std::string - Complete HTTP response for the GET/HEAD request.
 */
std::string	ClientConnection::methodGet( bool body )
{
	if (this->_HttpRequest.getRequest().size() > MAX_GET_LENGHT)
		return (this->uriTooLong());

	// Normalize URI and resolve the on-disk file path.
	std::string uri = this->_HttpRequest.getPath();
	if (uri.empty())
		uri = "/";

	std::string filePath = Router::resolvePath(uri, *this->_config);
	const RouteConfig* route = this->_route;
	if (!route)
	{
		route = Router::matchRoute(*this->_config, uri);
		this->_route = route;
	}

	struct stat st;
	if (stat(filePath.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
	{
		// Directories are only allowed when autoindex is explicitly on.
		if (route && route->autoindex)
			return (this->generateAutoindex(filePath));
		return (this->forbidden());
	}

	std::ifstream file(filePath.c_str());

	// Validate filesystem accessibility before streaming file contents.
	if (access(filePath.c_str(), F_OK) < 0)
		return (this->notFound());
	else if (access(filePath.c_str(), R_OK) < 0)
		return (this->forbidden());
	else if (!file.is_open())
		return (this->internalServerError());
	else if (CGIHandler::isCgiRequest(filePath, route))
	{
		if (route == NULL || route->cgiPath.empty())
			return (this->internalServerError());
		std::vector<std::string>	env =
			CGIHandler::buildEnvironment(this->_HttpRequest, filePath);
		std::vector<std::string>	args =
			CGIHandler::buildArguments(route, filePath);
		CGIHandler					cgi(env, args, this->_HttpRequest.getBody(), filePath);
		std::string output = this->_route->hasMaxBodySize ?
			cgi.run(this->_route->maxBodySize) : cgi.run(100000000);
		parseCgiResponse(output, this->_HttpResponse);
		return (this->getHeader() + this->_HttpResponse.getBody());
	}
	else
		return (this->ok(file, body));
}

/*
 * Function: ClientConnection::methodPost
 * Role: Handle POST uploads or CGI execution depending on route configuration.
 *
 * Returns:
 *   std::string - HTTP response produced after processing the POST body.
 */
std::string	ClientConnection::methodPost( void )
{
	size_t	maxBody = this->getMaxBodySize();
	size_t	length = this->_HttpRequest.getBody().size();
	if (!this->_HttpRequest.getHeader("Content-Length").empty())
		length = static_cast<size_t>(std::strtol(this->_HttpRequest.getHeader("Content-Length").c_str(), NULL, 10));
	if (maxBody > 0 && length > maxBody)
		return (this->error(413, "Payload Too Large"));

	std::string uri = this->_HttpRequest.getPath();
	if (uri.empty())
		uri = "/";

	std::string filePath = Router::resolvePath(uri, *this->_config);
	const RouteConfig* route = this->_route;
	if (!route)
	{
		route = Router::matchRoute(*this->_config, uri);
		this->_route = route;
	}

	if (route && !route->uploadDir.empty())
	{
		if (!this->ensureDirectoryExists(route->uploadDir))
			return (this->internalServerError());

		std::string uriPath = this->_HttpRequest.getPath();
		if (uriPath.empty() || uriPath[uriPath.size() - 1] == '/')
			return (this->badRequest());

		size_t pos = uriPath.find_last_of('/');
		std::string filename = uriPath.substr(pos + 1);
		if (filename.empty())
			return (this->badRequest());

		std::string baseDir = route->uploadDir;
		if (baseDir[baseDir.size() - 1] != '/')
			baseDir += '/';

		std::string finalPath = baseDir + filename;

		struct stat st;
		int version = 1;
		while (stat(finalPath.c_str(), &st) == 0)
		{
			size_t dot = filename.find_last_of('.');
			std::string name = (dot == std::string::npos) ? filename : filename.substr(0, dot);
			std::string ext = (dot == std::string::npos) ? "" : filename.substr(dot);
			std::ostringstream oss;
			oss << baseDir << name << "_" << version++ << ext;
			finalPath = oss.str();
		}

		std::ofstream out(finalPath.c_str(), std::ios::binary);
		if (!out.is_open())
			return (this->internalServerError());
		out << this->_HttpRequest.getBody();
		out.close();

		this->_HttpResponse.setStatusCode(201);
		this->_HttpResponse.setStatusText("Created");
		this->_HttpResponse.setBody("<html><body><h1>Upload OK</h1></body></html>");
		return (this->getHeader() + this->_HttpResponse.getBody());
	}

	std::ifstream file(filePath.c_str());
	if (access(filePath.c_str(), F_OK) < 0)
		return (this->notFound());
	else if (access(filePath.c_str(), R_OK) < 0)
		return (this->forbidden());
	else if (!file.is_open())
		return (this->internalServerError());
	else if (CGIHandler::isCgiRequest(filePath, route))
	{
		if (route == NULL || route->cgiPath.empty())
			return (this->internalServerError());
		std::vector<std::string>	env =
			CGIHandler::buildEnvironment(this->_HttpRequest, filePath);
		std::vector<std::string>	args =
			CGIHandler::buildArguments(route, filePath);
		CGIHandler					cgi(env, args, this->_HttpRequest.getBody(), filePath);
		std::string output = this->_route->hasMaxBodySize ?
			cgi.run(this->_route->maxBodySize) : cgi.run(100000000);

		parseCgiResponse(output, this->_HttpResponse);
		return (this->getHeader() + this->_HttpResponse.getBody());
	}
	else
		return (this->ok(file, true));
}

/*
 * Function: ClientConnection::methodDelete
 * Role: Remove a resource from disk if permissions allow.
 *
 * Returns:
 *   std::string - HTTP response reflecting the deletion result.
 */
std::string	ClientConnection::methodDelete(void)
{
	std::string uri = this->_HttpRequest.getPath();	// Extract requested URI.
	if (uri.empty())
		uri = "/";	// Normalize empty path.

	std::string filePath = Router::resolvePath(uri, *this->_config);	// Map URI → filesystem path.

	// If the file does not exist → cannot delete it.
	if (access(filePath.c_str(), F_OK) < 0)
		return (this->notFound());	// 404 Not Found.

	// If the file exists but cannot be written/removed.
	else if (access(filePath.c_str(), W_OK) < 0)
		return (this->forbidden());	// 403 Forbidden.

	// File exists AND is writable → delete it.
	else
	{
		std::remove(filePath.c_str());	// Perform file deletion.
		return (this->noContent());		// 204 No Content (successful DELETE).
	}
}

/*
 * Function: ClientConnection::methodPut
 * Role: Create or overwrite a resource with the request body.
 *
 * Returns:
 *   std::string - HTTP response describing creation or update outcome.
 */
std::string	ClientConnection::methodPut(void)
{
	size_t	maxBody = this->getMaxBodySize();															// Max allowed body size (route > server).
	size_t	length = std::strtol(this->_HttpRequest.getHeader("Content-Length").c_str(), NULL, 10);		// Declared incoming body size.

	// Reject if Content-Length exceeds the limit.
	if (maxBody > 0 && length > maxBody)
		return (this->error(413, "Payload Too Large"));

	// Normalize URI (fallback to "/" if empty).
	std::string uri = this->_HttpRequest.getPath();
	if (uri.empty())
		uri = "/";

	// Resolve URI into final filesystem path.
	std::string filePath = Router::resolvePath(uri, *this->_config);

	// ----------- PUT CREATION OR REPLACEMENT LOGIC -----------

	// If file doesn't exist → create new file → return 201 Created.
	if (access(filePath.c_str(), F_OK) < 0)
	{
		std::ofstream newFile(filePath.c_str());	// Create new file.
		newFile << this->_HttpRequest.getBody();	// Write body into it.
		newFile.close();
		return (this->created());	// Respond 201.
	}
	// File exists but cannot be written.
	else if (access(filePath.c_str(), W_OK) < 0)
		return (this->forbidden());	// Respond 403.
	// File exists and is writable → overwrite it.
	else
	{
		std::ofstream oldFile(filePath.c_str(),
			std::ios::out | std::ios::trunc);	// Truncate+overwrite existing file.
		oldFile << this->_HttpRequest.getBody();
		oldFile.close();
		return (this->noContent());	// Respond 204
	}
}

/*
 * Function: ClientConnection::ok
 * Role: Build a 200 OK response streaming the provided file if needed.
 *
 * Parameters:
 *   file - Open file stream positioned at the start of the resource.
 *   body - true to include the body (GET), false for HEAD semantics.
 *
 * Returns:
 *   std::string - Serialized HTTP response.
 */
std::string	ClientConnection::ok( std::ifstream& file, bool body)
{
	std::stringstream	ss;

	// Read the entire file into memory so headers can reference its size.
	ss << file.rdbuf();
	this->_HttpResponse.setStatusCode(200);
	this->_HttpResponse.setStatusText("OK");
	this->_HttpResponse.setBody(ss.str());
	if (body)
		return (this->getHeader() + this->_HttpResponse.getBody());
	else
		return (this->getHeader());
}

/*
 * Function: ClientConnection::noContent
 * Role: Produce a 204 No Content response.
 *
 * Returns:
 *   std::string - Serialized HTTP response with no body.
 */
std::string	ClientConnection::noContent(void)
{
	this->_HttpResponse.setStatusCode(204);
	this->_HttpResponse.setStatusText("No Content");
	return (this->getHeader());
}

/*
 * Function: ClientConnection::created
 * Role: Build a 201 Created response.
 *
 * Returns:
 *   std::string - Serialized HTTP response headers.
 */
std::string	ClientConnection::created(void)
{
	this->_HttpResponse.setStatusCode(201);
	this->_HttpResponse.setStatusText("Created");
	return (this->getHeader());
}
