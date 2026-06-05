/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:35:01 by jguillem          #+#    #+#             */
/*   Updated: 2025/11/20 14:21:26 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_CONNECTION_HPP
# define CLIENT_CONNECTION_HPP

# include "all.hpp"

/*==============================================================*/
/*                       ClientConnection                       */
/*==============================================================*/
/* Description: Manages the lifecycle and response logic for a single client socket. */

class ClientConnection
{
	public:
		/*===========================*/
		/*  CONSTRUCTORS/DESTRUCTORS */
		/*===========================*/
		ClientConnection(int client_fd, const ServerConfig& cfg);	// Bind socket to configuration
		~ClientConnection(void);										// Release resources

		/*===========================*/
		/*       PUBLIC METHODS      */
		/*===========================*/
		bool			readRequest(void);                              // Read data from client socket
		void			resetRequest(void);                             // Clear accumulated request state
		void			closeConnection(void);                          // Close underlying socket
		bool			isClosed(void);                                 // Check if connection is closed
		bool			shouldClose(void);                              // Check keep-alive policy
		bool			hasResponseReady(void) const;                   // Determine if buffered response is ready
		int				sendResponse(void);                             // Flush queued response to client
		void			queueResponse(const std::string& response);     // Store response payload for sending

		std::string		etablishResponse(void);                         // Build response based on parsed request
		std::string		methodGet(bool body);                           // Handle GET & HEAD requests
		std::string		methodPost(void);                               // Handle POST requests
		std::string		methodDelete(void);                             // Handle DELETE requests
		std::string		methodPut(void);                             	// Handle PUT requests
		std::string		ok(std::ifstream& file, bool body);             // Build successful response from file
		
		std::string		error(int statusCode, std::string statusText);  // Generate generic error response
		std::string		getHeader(void);                                // Build common response headers
		std::string		badRequest(void);                               // 400 error helper
		std::string		notFound(void);                                 // 404 error helper
		std::string		noContent(void);								// 204 error helper	
		std::string		created(void);									// 201 error helper	
		std::string		notImplemented(void);                           // 501 error helper
		std::string		notAllowedMethod(void);                         // 405 error helper
		std::string		uriTooLong(void);                               // 414 error helper
		std::string		forbidden(void);                                // 403 error helper
		std::string		internalServerError(void);                      // 500 error helper
		std::string		gatewayTimeout(void);                           // 504 error helper

	private:
		/*===========================*/
		/*         ATTRIBUTES        */
		/*===========================*/
		int									_fd;            // Client socket descriptor
		const ServerConfig*					_config;        // Active server configuration
		const RouteConfig*					_route;			// Active route
		HttpRequest							_HttpRequest;   // Incoming HTTP request parser
		HttpResponse						_HttpResponse;  // Outgoing HTTP response builder
		bool								_closed;        // Connection closure state
		bool								_keepAlive;     // Whether connection stays open after response
		std::string							_responseBuffer;// Pending response bytes
		size_t								_bytesSent;     // Number of bytes already sent
		/*===========================*/
		/*        HELPERS            */
		/*===========================*/
		std::string		redirectResponse(int statusCode,
						const std::string& url);								// Build HTTP redirect response
		std::string		generateAutoindex(const std::string& dirPath);			// Build listing for directories
		bool			ensureDirectoryExists(const std::string& path) const;	// Create directories recursively
		size_t			getMaxBodySize(void) const;								// Resolve active body-size limit
};

#endif
