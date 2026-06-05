/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection_Errors.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:00:00 by alafon            #+#    #+#             */
/*   Updated: 2025/11/16 18:32:45 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

/*==============================================================*/
/*                        ERROR HANDLING                        */
/*==============================================================*/

/*
 * Function: ClientConnection::error
 * Role: Construct an error response optionally backed by a custom error page.
 *
 * Parameters:
 *   statusCode - HTTP status to send.
 *   statusText - Status phrase describing the error.
 *
 * Returns:
 *   std::string - Serialized error response.
 */
std::string	ClientConnection::error(int statusCode, std::string statusText)
{
	std::string	pagePath = Router::resolveError(statusCode, *this->_config);	// Determine which error page to serve.
	std::ifstream	file(pagePath.c_str());										// Try to open the custom error page.
	std::stringstream	ss;														// Will build the error response body.

	if (file.is_open())
		ss << file.rdbuf();	// Use the custom error file content.
	else
	{
		// Fallback: generate a minimal built-in HTML error page.
		ss << "<html><head><title>" << statusCode << " " << statusText;
		ss << "</title></head><body><center><h1>" << statusCode << " " << statusText;
		ss << "</h1></center><hr><center>webserv/1.0</center></body></html>\r\n";
	}

	// Fill HTTP response fields accordingly.
	this->_HttpResponse.setStatusCode(statusCode);
	this->_HttpResponse.setStatusText(statusText);
	this->_HttpResponse.setBody(ss.str());									// Store final error body.

	return (this->getHeader() + this->_HttpResponse.getBody());				// Return full HTTP response.
}

/*
 * Function: ClientConnection::badRequest
 * Role: Wrapper that returns a 400 Bad Request response.
 *
 * Returns:
 *   std::string - Serialized HTTP error message.
 */
std::string	ClientConnection::badRequest( void )
{
	return (this->error(400, "Bad Request"));
}

/*
 * Function: ClientConnection::forbidden
 * Role: Build a 403 Forbidden response.
 *
 * Returns:
 *   std::string - Serialized HTTP error message.
 */
std::string	ClientConnection::forbidden( void )
{
	return (this->error(403, "Forbidden"));
}

/*
 * Function: ClientConnection::notFound
 * Role: Build a 404 Not Found response.
 *
 * Returns:
 *   std::string - Serialized HTTP error message.
 */
std::string	ClientConnection::notFound( void )
{
	return (this->error(404, "Not Found"));
}

/*
 * Function: ClientConnection::notAllowedMethod
 * Role: Build a 405 Method Not Allowed response.
 *
 * Returns:
 *   std::string - Serialized HTTP error message.
 */
std::string	ClientConnection::notAllowedMethod( void )
{
	return (this->error(405, "Method Not Allowed"));
}

/*
 * Function: ClientConnection::uriTooLong
 * Role: Build a 414 URI Too Long response.
 *
 * Returns:
 *   std::string - Serialized HTTP error message.
 */
std::string	ClientConnection::uriTooLong( void )
{
	return (this->error(414, "URI Too Long"));
}

/*
 * Function: ClientConnection::internalServerError
 * Role: Build a 500 Internal Server Error response.
 *
 * Returns:
 *   std::string - Serialized HTTP error message.
 */
std::string	ClientConnection::internalServerError( void )
{
	return (this->error(500, "Internal Server Error"));
}

/*
 * Function: ClientConnection::gatewayTimeout
 * Role: Build a 504 Gateway Timeout response.
 *
 * Returns:
 *   std::string - Serialized HTTP error message.
 */
std::string	ClientConnection::gatewayTimeout( void )
{
	return (this->error(504, "Gateway Timeout"));
}

/*
 * Function: ClientConnection::notImplemented
 * Role: Build a 501 Not Implemented response.
 *
 * Returns:
 *   std::string - Serialized HTTP error message.
 */
std::string	ClientConnection::notImplemented( void )
{
	return (this->error(501, "Not Implemented"));
}

/*
 * Function: ClientConnection::getHeader
 * Role: Serialize the HTTP response start-line and headers currently set.
 *
 * Returns:
 *   std::string - Header section terminated by the final CRLF pair.
 */
std::string	ClientConnection::getHeader(void)
{
	std::stringstream stream;	// Buffer to assemble the full HTTP header.

	// Status line: "HTTP/1.1 200 OK"
	stream << this->_HttpResponse.getProtocol() << " "
	       << this->_HttpResponse.getStatusCode() << " "
	       << this->_HttpResponse.getStatusText() << "\r\n";

	// Basic server information.
	stream << "Server: webserv/1.0\r\n";

	// Always include Content-Length so the client knows how many bytes to read.
	stream << "Content-Length: "
	       << this->_HttpResponse.getBody().size() << "\r\n";

	// Content-Location for 201 (Created) or 204 (No Content), except on DELETE.
	if (this->_HttpRequest.getMethod() != "DELETE" &&
		(this->_HttpResponse.getStatusCode() == 201 ||
		 this->_HttpResponse.getStatusCode() == 204))
	{
		stream << "Content-Location: "
		       << this->_HttpRequest.getPath() << "\r\n";	// Reflect resource location.
	}

	// Add Content-Type for successful (200) responses when MIME type was detected.
	if (this->_HttpResponse.getStatusCode() == 200 &&
		!this->_HttpRequest.getMimeType().empty())
	{
		stream << "Content-Type: "
		       << this->_HttpRequest.getMimeType() << "\r\n";	// e.g., text/html, image/png, etc.
	}

	// Keep-alive only if requested AND response isn't an error.
	if (this->_keepAlive && this->_HttpResponse.getStatusCode() < 400)
		stream << "Connection: keep-alive\r\n";	// Reuse connection.
	else
		stream << "Connection: close\r\n";		// Close after this response.

	stream << "\r\n";		// Empty line ends header section.
	return (stream.str());	// Return assembled header.
}

/*
 * Helper: defaultRedirectReason
 * Role: Provide a human-readable status text for common redirect codes.
 */
static std::string	defaultRedirectReason(int statusCode)
{
	switch (statusCode)
	{
		case 300: return ("Multiple Choices");
		case 301: return ("Moved Permanently");
		case 302: return ("Found");
		case 303: return ("See Other");
		case 304: return ("Not Modified");
		case 305: return ("Use Proxy");
		case 307: return ("Temporary Redirect");
		case 308: return ("Permanent Redirect");
		default: break;
	}
	if (statusCode >= 300 && statusCode < 400)
		return ("Redirect");
	return ("Found");
}

/*
 * Function: ClientConnection::redirectResponse
 * Role: Generate a minimal HTTP redirect response honoring the configured status/url.
 *
 * Parameters:
 *   statusCode - HTTP status code to send (301, 302, 307, etc.).
 *   url - Absolute or relative URL supplied by the configuration.
 *
 * Returns:
 *   std::string - Serialized HTTP redirect response with Location header.
 */
std::string	ClientConnection::redirectResponse(int statusCode,
					const std::string& url)
{
	std::string statusText = defaultRedirectReason(statusCode);
	this->_HttpResponse.setStatusCode(statusCode);
	this->_HttpResponse.setStatusText(statusText);
	this->_HttpResponse.setBody("");

	std::stringstream stream;
	stream << this->_HttpResponse.getProtocol() << " "
	       << statusCode << " " << this->_HttpResponse.getStatusText() << "\r\n";
	stream << "Server: webserv/1.0\r\n";
	stream << "Location: " << url << "\r\n";
	stream << "Content-Length: 0\r\n";
	stream << "Connection: " << (this->_keepAlive ? "keep-alive" : "close") << "\r\n";
	stream << "\r\n";
	return (stream.str());
}
