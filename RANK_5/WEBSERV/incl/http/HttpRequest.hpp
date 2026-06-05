/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex                                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:07:53 by alex              #+#    #+#             */
/*   Updated: 2025/11/09 16:55:34 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include "all.hpp"

# define MAX_GET_LENGHT 2048

/*==============================================================*/
/*                           HttpRequest                        */
/*==============================================================*/
/* Description: Parses and stores data for a single HTTP request message. */

class HttpRequest
{
	public:
		/*===========================*/
		/*       CONSTRUCTORS        */
		/*===========================*/
		HttpRequest(void);
		~HttpRequest(void);

		/*===========================*/
		/*       PUBLIC METHODS      */
		/*===========================*/
		bool								getIsGoodSyntax(void);                   // Report whether parsing succeeded
		std::string&						getMethod(void);                        // Access request method
		std::string&						getPath(void);                          // Access request path
		std::string&						getBody(void);                          // Access request body
		std::string&						getQuery(void);                         // Access query string portion
		std::string&						getRequest(void);                       // Access raw request buffer
		std::string							getHeader(const std::string& key);      // Lookup a header value
		std::map<std::string, std::string>&	getHeaders(void);                       // Access header map
		std::string							getMimeType(void);						// Detect mime type

		void								setIsGoodSyntax(bool goodSyntax);       // Update syntax flag

		bool								parse(const std::string& raw_request);  // Parse a raw HTTP request
		void								appendToRequest(const char* data, size_t len); // Append raw bytes to buffer
		void								clear(void);                            // Reset parser state

	private:
		/*===========================*/
		/*         ATTRIBUTES        */
		/*===========================*/
		std::string							_request;        // Accumulated raw request buffer
		std::string							_method;         // Parsed HTTP method
		std::string							_path;           // Parsed path component
		std::string							_version;        // Parsed HTTP version
		std::string							_body;           // Parsed message body
		std::string							_query;          // Parsed query string
		std::map<std::string, std::string>	_mimeTypes;		 // Parsed mime types
		std::map<std::string, std::string>	_headers;        // Parsed headers
		bool								_isGoodSyntax;   // Indicates parsing success
};

#endif
