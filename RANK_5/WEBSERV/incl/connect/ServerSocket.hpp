/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 18:58:39 by jguillem          #+#    #+#             */
/*   Updated: 2025/11/06 01:09:18 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP

# include "all.hpp"

/*==============================================================*/
/*                          ServerSocket                        */
/*==============================================================*/
/* Description: Wraps a listening socket used to accept incoming client connections. */

class ServerSocket
{
	public:
		/*===========================*/
		/*  CONSTRUCTORS/DESTRUCTORS */
		/*===========================*/
		ServerSocket(const ServerConfig& cfg);      // Construct from server configuration
		~ServerSocket(void);                        // Close socket on destruction

		/*===========================*/
		/*       PUBLIC METHODS      */
		/*===========================*/
		void	parameterize(uint16_t port);        // Configure socket options and address
		void	bindAndListen(void);                // Bind and start listening for clients
		int		acceptConnection(void);             // Accept a new client connection
		int		getFd(void);                        // Retrieve the socket file descriptor
		const ServerConfig& getConfig(void) const;  // Access the stored configuration

	private:
		/*===========================*/
		/*        ATTRIBUTES         */
		/*===========================*/
		int					_fd;        // Listening socket descriptor
		struct sockaddr_in	_address;   // Socket address information
		ServerConfig		_config;    // Associated server configuration
};

#endif
