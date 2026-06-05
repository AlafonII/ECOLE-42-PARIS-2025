/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection_Utils.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:00:00 by alafon            #+#    #+#             */
/*   Updated: 2025/11/13 14:28:15 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

/*==============================================================*/
/*                          UTILITIES                           */
/*==============================================================*/

/*
 * Function: ClientConnection::generateAutoindex
 * Role: Produce a simple HTML directory listing when autoindexing is enabled.
 *
 * Parameters:
 *   dirPath - Filesystem directory whose contents are listed.
 *
 * Returns:
 *   std::string - Complete HTTP response describing the directory.
 */
std::string	ClientConnection::generateAutoindex(const std::string& dirPath)
{
	DIR* dir = opendir(dirPath.c_str());
	if (!dir)
		return (this->error(404, "Not Found"));

	std::stringstream body;
	body << "<html><head><title>Index of " << dirPath << "</title></head><body>";
	body << "<h1>Index of " << dirPath << "</h1><ul>";

	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL)
	{
		// List each entry except "." and annotate directories with slash.
		std::string name = entry->d_name;
		if (name == ".")
			continue;
		std::string href = name;
		if (entry->d_type == DT_DIR)
			href += "/";
		body << "<li><a href=\"" << href << "\">" << name << "</a></li>";
	}

	body << "</ul></body></html>";
	closedir(dir);

	std::stringstream response;
	response << "HTTP/1.1 200 OK\r\n"
	         << "Content-Type: text/html\r\n"
	         << "Content-Length: " << body.str().size() << "\r\n"
	         << "\r\n"
	         << body.str();
	return (response.str());
}


/*
 * Function: ClientConnection::ensureDirectoryExists
 * Role: Verify that the provided path already exists and is a directory.
 *
 * Parameters:
 *   path - Filesystem path that should resolve to an existing directory.
 *
 * Returns:
 *   bool - true if the directory exists and is a directory.
 */
bool	ClientConnection::ensureDirectoryExists(const std::string& path) const
{
	if (path.empty())
		return (false);

	struct stat st;
	if (stat(path.c_str(), &st) != 0)
		return (false);
	return (S_ISDIR(st.st_mode));
}


/*
 * Function: ClientConnection::getMaxBodySize
 * Role: Determine the active body-size limit considering route overrides.
 *
 * Returns:
 *   size_t - Maximum allowed body length in bytes (0 if unlimited).
 */
size_t	ClientConnection::getMaxBodySize(void) const
{
	// Route-specific overrides take precedence over global settings.
	if (this->_route && this->_route->hasMaxBodySize)
		return (this->_route->maxBodySize);
	if (this->_config)
		return (this->_config->clientMaxBodySize);
	return (0);
}


/*
 * Function: toLower
 * Role: Produce a lowercase version of the provided string.
 *
 * Parameters:
 *   s - Source string to normalize.
 *
 * Returns:
 *   std::string - Lowercase copy of the input string.
 */
std::string	toLower(const std::string& s)
{
	std::string out = s;
	for (size_t i = 0; i < out.size(); i++)
	{
		if (out[i] >= 'A' && out[i] <= 'Z')
			out[i] = static_cast<char>(out[i] - 'A' + 'a');
	}
	return out;
}


/*
 * Function: getContentLength
 * Role: Extract the Content-Length header from a raw HTTP request.
 *
 * Parameters:
 *   raw - Raw HTTP request string containing headers and possibly a body.
 *
 * Returns:
 *   long long - Parsed Content-Length value, or -1 if missing/invalid.
 */
long long	getContentLength(const std::string& raw)
{
	size_t	headerEnd = raw.find("\r\n\r\n");
	if (headerEnd == std::string::npos)
		return (-1);

	std::string			headers = raw.substr(0, headerEnd);
	std::istringstream	stream(headers);
	std::string			line;

	if (!std::getline(stream, line))
		return (-1);
	while (std::getline(stream, line))
	{
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		if (line.empty())
			break;
		size_t	colon = line.find(':');
		if (colon == std::string::npos)
			continue;
		std::string key = line.substr(0, colon);
		for (size_t i = 0; i < key.size(); ++i)
			key[i] = static_cast<char>(std::tolower(key[i]));
		if (key == "content-length")
		{
			std::string	value = line.substr(colon + 1);
			while (!value.empty() && std::isspace(static_cast<unsigned char>(value[0])))
				value.erase(0, 1);
			long long	result = -1;
			std::istringstream converter(value);
			if ((converter >> result) && result >= 0)
				return (result);
			break;
		}
	}
	return (-1);
}
