/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 18:51:19 by jguillem          #+#    #+#             */
/*   Updated: 2025/11/19 19:01:57 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

/*==============================================================*/
/*                        CONSTRUCTORS                          */
/*==============================================================*/

/*
 * Function: ServerSocket::ServerSocket
 * Role: Build the socket using a full server configuration object.
 *
 * Parameters:
 *   cfg - Server configuration to mirror.
 */
ServerSocket::ServerSocket(const ServerConfig& cfg)
{
	std::memset(&this->_address, 0, sizeof(this->_address));
	this->_fd = -1;
	this->_config = cfg;
	this->parameterize(cfg.port);
}


/*==============================================================*/
/*                        DESTRUCTORS                           */
/*==============================================================*/

/*
 * Function: ServerSocket::~ServerSocket
 * Role: Close the listening socket when the object goes out of scope.
 */
ServerSocket::~ServerSocket(void)
{
	if (this->_fd >= 0)
		close(this->_fd);
}


/*==============================================================*/
/*         SOCKET INITIALIZATION / CONNECTION MANAGEMENT        */
/*==============================================================*/

/*
 * Function: ServerSocket::parameterize
 * Role: Create the socket descriptor and configure its address/option state.
 *
 * Parameters:
 *   port - Port number to encode in the sockaddr structure.
 */
void	ServerSocket::parameterize(uint16_t port)
{
	int opt = 1;

	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0)
		throw std::runtime_error("socket failure");

	int flags = fcntl(this->_fd, F_GETFL, 0);
	if (flags < 0 || fcntl(this->_fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		close(this->_fd);
		throw std::runtime_error("failed to enable non-blocking mode");
	}

	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR,
		&opt, sizeof(opt)) < 0)
	{
		close(this->_fd);
		throw std::runtime_error("setsockopt(SO_REUSEADDR) failure");
	}
#ifdef SO_REUSEPORT
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEPORT,
		&opt, sizeof(opt)) < 0)
	{
		close(this->_fd);
		throw std::runtime_error("setsockopt(SO_REUSEPORT) failure");
	}
#endif

	this->_address.sin_family      = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port        = htons(port);
	this->_config.port             = port;
}

/*
 * Function: ServerSocket::bindAndListen
 * Role: Bind the socket to the configured address and start listening.
 */
void	ServerSocket::bindAndListen(void)
{
	if (bind(this->_fd, (struct sockaddr*)&this->_address, sizeof(this->_address)) < 0)
	{
		std::cerr << strerror(errno) << std::endl;
		std::exit(EXIT_FAILURE);
	}
	// Begin listening with a modest backlog sufficient for the project requirements.
	if (listen(this->_fd, 1024) < 0)
	{
		std::cerr << strerror(errno) << std::endl;
		std::exit(EXIT_FAILURE);
	}
}


/*
 * Function: ServerSocket::acceptConnection
 * Role: Accept a pending client connection and return its descriptor.
 *
 * Returns:
 *   int - Newly accepted client socket descriptor, or -1 on failure.
 */
int ServerSocket::acceptConnection(void)
{
    sockaddr_in clientAddr;
    socklen_t   addrlen = sizeof(clientAddr);

    // Accept a new client connection from the listening socket.
    int new_socket = accept(this->_fd,
                            reinterpret_cast<sockaddr*>(&clientAddr),
                            &addrlen);

    // If the accept call fails, log the issue and return -1.
    if (new_socket < 0)
    {
        std::cerr << "accept() failed: unable to accept connection" << std::endl;
        return -1;    // Caller must ignore this and continue the poll loop.
    }

    return new_socket;
}

/*==============================================================*/
/*                            GETTERS                           */
/*==============================================================*/

/*
 * Function: ServerSocket::getFd
 * Role: Expose the listening socket descriptor for polling.
 *
 * Returns:
 *   int - Listening socket file descriptor.
 */
int	ServerSocket::getFd(void)
{
	return (this->_fd);
}


/*
 * Function: ServerSocket::getConfig
 * Role: Provide read-only access to the socket's configuration.
 *
 * Returns:
 *   const ServerConfig& - Reference to the stored configuration.
 */
const ServerConfig& ServerSocket::getConfig(void) const {
	return this->_config;
}
