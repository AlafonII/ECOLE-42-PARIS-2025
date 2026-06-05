/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:50:12 by jguillem          #+#    #+#             */
/*   Updated: 2025/11/09 15:49:57 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

/*==============================================================*/
/*                        CONSTRUCTORS                          */
/*==============================================================*/

/*
 * Function: HttpResponse::HttpResponse
 * Role: Initialize default protocol and empty response fields.
 */
HttpResponse::HttpResponse( void ): _protocol("HTTP/1.1")
{
	return;
}


/*==============================================================*/
/*                        DESTRUCTORS                           */
/*==============================================================*/

/*
 * Function: HttpResponse::~HttpResponse
 * Role: Default destructor for the response object.
 */
HttpResponse::~HttpResponse( void )
{
	return;
}


/*==============================================================*/
/*                          SETTERS                             */
/*==============================================================*/

/*
 * Function: HttpResponse::setBody
 * Role: Assign the serialized body content of the response.
 *
 * Parameters:
 *   body - Fully rendered body string.
 */
void	HttpResponse::setBody( const std::string& body )
{
	// Replace any previous body payload with the new data.
	this->_body = body;
}


/*
 * Function: HttpResponse::setStatusText
 * Role: Store the textual description for the current status code.
 *
 * Parameters:
 *   statusText - Status phrase such as "OK".
 */
void	HttpResponse::setStatusText( const std::string& statusText )
{
	this->_statusText = statusText;
}


/*
 * Function: HttpResponse::setStatusCode
 * Role: Update the numeric HTTP status.
 *
 * Parameters:
 *   statusCode - HTTP status integer.
 */
void	HttpResponse::setStatusCode( int statusCode )
{
	this->_statusCode = statusCode;
}

/*==============================================================*/
/*                          GETTERS                             */
/*==============================================================*/

/*
 * Function: HttpResponse::getProtocol
 * Role: Retrieve the protocol string (e.g., HTTP/1.1).
 *
 * Returns:
 *   std::string - Stored protocol value.
 */
std::string	HttpResponse::getProtocol( void )
{
	return (this->_protocol);
}


/*
 * Function: HttpResponse::getStatusCode
 * Role: Retrieve the numeric HTTP status value.
 *
 * Returns:
 *   int - Stored status code.
 */
int	HttpResponse::getStatusCode( void )
{
	return (this->_statusCode);
}


/*
 * Function: HttpResponse::getStatusText
 * Role: Retrieve the textual status string.
 *
 * Returns:
 *   std::string - Stored status phrase.
 */
std::string	HttpResponse::getStatusText( void )
{
	return (this->_statusText);
}


/*
 * Function: HttpResponse::getBody
 * Role: Retrieve the serialized response body.
 *
 * Returns:
 *   std::string - Stored body string.
 */
std::string	HttpResponse::getBody( void )
{
	return (this->_body);
}
