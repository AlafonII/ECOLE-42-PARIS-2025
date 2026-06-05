/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 17:41:42 by jguillem          #+#    #+#             */
/*   Updated: 2025/11/21 13:04:42 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

std::string	toLower(const std::string& s);
long long	getContentLength(const std::string& raw);

/*==============================================================*/
/*                        CONSTRUCTORS                          */
/*==============================================================*/

/*
 * Function: ClientConnection::ClientConnection
 * Role: Bind a client socket descriptor to its server configuration context.
 *
 * Parameters:
 *   client_fd - Accepted client socket descriptor.
 *   cfg - Server configuration that governs this connection.
 */
ClientConnection::ClientConnection(int client_fd,
	const ServerConfig& cfg)
	: _fd(client_fd)
	, _config(&cfg)
	, _route(NULL)
	, _closed(false)
	, _keepAlive(true)
	, _responseBuffer()
	, _bytesSent(0)
{
	// Switch the client socket to non-blocking to integrate with poll()
	fcntl(this->_fd, F_SETFL, O_NONBLOCK);
}

/*==============================================================*/
/*                        DESTRUCTORS                           */
/*==============================================================*/

/*
 * Function: ClientConnection::~ClientConnection
 * Role: Default destructor; sockets are closed elsewhere before destruction.
 */
ClientConnection::~ClientConnection( void )
{
	return;
}

/*==============================================================*/
/*                  CONNECTION MANAGEMENT                       */
/*==============================================================*/

/*
 * Function: ClientConnection::readRequest
 * Role: Read pending data from the client socket and determine if the request is complete.
 *
 * Returns:
 *   bool - true when the full request (headers and body) has been received.
 */
bool	ClientConnection::readRequest( void )
{
	char	buffer[1024] = {0};
	ssize_t	bytes;

	// Pull available bytes from the non-blocking client socket
	bytes = read(this->_fd, buffer, 1023);
	if (bytes == 0)
	{
		this->_closed = true;
		return (false);
	}
	if (bytes < 0)
	{
		this->_closed = true;
		return (false);
	}

	// Accumulate the raw bytes into the HttpRequest cache
	this->_HttpRequest.appendToRequest(buffer, static_cast<size_t>(bytes));

	std::string& raw = this->_HttpRequest.getRequest();
	size_t headerEnd = raw.find("\r\n\r\n");
	if (headerEnd == std::string::npos)
		return (false);

	// If Content-Length is missing or zero, headers alone are enough
	long long	contentLength = getContentLength(raw);

	/* Handle chunked transfer-coding completion */
	std::string	headers = raw.substr(0, headerEnd);
	std::string	lowerHeaders = headers;
	for (size_t i = 0; i < lowerHeaders.size(); ++i)
	{
		if (lowerHeaders[i] >= 'A' && lowerHeaders[i] <= 'Z')
			lowerHeaders[i] = static_cast<char>(lowerHeaders[i] - 'A' + 'a');
	}
	if (lowerHeaders.find("transfer-encoding: chunked") != std::string::npos)
	{
		if (raw.size() >= 5 &&
			raw.compare(raw.size() - 5, 5, "0\r\n\r\n") == 0)
			return (true);
		return (false);
	}

	if (contentLength <= 0)
		return (true);

	// Ensure the buffered body meets the declared Content-Length
	size_t	bodyLen = raw.size() - (headerEnd + 4);
	return (bodyLen >= static_cast<size_t>(contentLength));
}

/*
 * Function: ClientConnection::resetRequest
 * Role: Clear the stored HttpRequest state after a response is sent.
 */
void	ClientConnection::resetRequest( void )
{
	// Drop accumulated buffers so the next request starts fresh.
	this->_HttpRequest.clear();
	return;
}

/*
 * Function: ClientConnection::queueResponse
 * Role: Store a serialized HTTP response for asynchronous transmission.
 *
 * Parameters:
 *   response - Fully formatted HTTP response string.
 */
void	ClientConnection::queueResponse( const std::string& response )
{
	// Reset the send offset whenever a new response is prepared.
	this->_responseBuffer = response;
	this->_bytesSent = 0;
}

/*
 * Function: ClientConnection::sendResponse
 * Role: Flush the queued HTTP response to the client in a non-blocking manner.
 */
int	ClientConnection::sendResponse()
{
	if (this->_responseBuffer.empty())
		return (0);

	// Attempt to send the remaining portion of the response buffer
	ssize_t	sent = write(this->_fd,
					this->_responseBuffer.c_str() + this->_bytesSent,
					this->_responseBuffer.size() - this->_bytesSent);
	if (sent < 0)
	{
		this->closeConnection();
		return (1);
	}
	this->_bytesSent += sent;

	if (this->_bytesSent >= _responseBuffer.size())
	{
		// Entire response has been transmitted; clear buffer for next request
		this->_responseBuffer.clear();
		this->_bytesSent = 0;
	}
	return (0);
}

/*
 * Function: ClientConnection::closeConnection
 * Role: Close the client socket and mark the connection as terminated.
 */
void	ClientConnection::closeConnection(void)
{
	if (!this->_closed)
	{
		close(this->_fd);
		this->_closed = true;
	}
}

/*==============================================================*/
/*                        STATE QUERIES                         */
/*==============================================================*/

/*
 * Function: ClientConnection::hasResponseReady
 * Role: Indicate whether there is buffered response data awaiting transmission.
 *
 * Returns:
 *   bool - true if the response buffer is non-empty.
 */
bool	ClientConnection::hasResponseReady( void ) const
{
	return (!this->_responseBuffer.empty());
}

/*
 * Function: ClientConnection::isClosed
 * Role: Report whether the underlying socket has been closed.
 *
 * Returns:
 *   bool - true if the connection is already closed.
 */
bool	ClientConnection::isClosed(void)
{
	return (this->_closed);
}

/*
 * Function: ClientConnection::shouldClose
 * Role: Determine keep-alive policy for the current connection.
 *
 * Returns:
 *   bool - true if the connection should be terminated after response.
 */
bool	ClientConnection::shouldClose(void)
{
	return (!this->_keepAlive);
}

/*==============================================================*/
/*               HTTP RESPONSE BUILDING                         */
/*==============================================================*/

/*
 * Function: ClientConnection::etablishResponse
 * Role: Parse the buffered request, route it, and dispatch to the correct method handler.
 *
 * Returns:
 *   std::string - Fully built HTTP response string.
 */
std::string	ClientConnection::etablishResponse( void )
{
	bool ok = this->_HttpRequest.parse(this->_HttpRequest.getRequest());
	this->_HttpRequest.setIsGoodSyntax(ok);
	if (!this->_HttpRequest.getIsGoodSyntax())
		return(this->badRequest());

	this->_route = Router::matchRoute(*this->_config, this->_HttpRequest.getPath());
	std::string conn = this->_HttpRequest.getHeader("Connection");
	if (!conn.empty())
	{
		if (toLower(conn) == "close")
			this->_keepAlive = false;
		else
			this->_keepAlive = true;
	}

	if (this->_route && this->_route->redirect_status != 0)
		return (this->redirectResponse(this->_route->redirect_status,
				this->_route->redirect_url));

	if (this->_route && !this->_route->allowedMethods.empty())
	{
		std::string method = this->_HttpRequest.getMethod();
		if (std::find(this->_route->allowedMethods.begin(),
				this->_route->allowedMethods.end(), method) ==
				this->_route->allowedMethods.end())
			return (this->notAllowedMethod());
	}

	std::string m = this->_HttpRequest.getMethod();
	if (m == "GET")
		return (this->methodGet(true));
	else if (m == "POST")
		return (this->methodPost());
	else if (m == "DELETE")
		return (this->methodDelete());
	else if (m == "PUT")
		return (this->methodPut());
	else if (m == "HEAD")
		return (this->methodGet(false));
	else
		return (this->notImplemented());
}
