/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:35:43 by jguillem          #+#    #+#             */
/*   Updated: 2025/11/09 16:13:08 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include "all.hpp"

/*==============================================================*/
/*                          HttpResponse                        */
/*==============================================================*/
/* Description: Builds HTTP responses and stores headers/body for transmission. */

class HttpResponse
{
	public:
		/*===========================*/
		/*  CONSTRUCTORS/DESTRUCTORS */
		/*===========================*/
		HttpResponse(void);                       // Initialize response with defaults
		~HttpResponse(void);                      // Clean up resources

		/*===========================*/
		/*       PUBLIC METHODS      */
		/*===========================*/
		void			setBody(const std::string& body);       // Set response body payload
		void			setStatusText(const std::string& statusText); // Set textual status message
		void			setStatusCode(int statusCode);          // Set numeric status code

		std::string		getProtocol(void);                      // Retrieve HTTP protocol string
		int				getStatusCode(void);                    // Retrieve status code
		std::string		getStatusText(void);                    // Retrieve status text
		std::string		getBody(void);                          // Retrieve body content

	private:
		/*===========================*/
		/*        ATTRIBUTES         */
		/*===========================*/
		std::string							_protocol;     // HTTP protocol version
		std::string							_body;         // Response body content
		int									_statusCode;   // Numeric status code
		std::string							_statusText;   // Textual status description
};

#endif
