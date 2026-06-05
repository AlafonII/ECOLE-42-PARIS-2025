/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 13:04:01 by jguillem          #+#    #+#             */
/*   Updated: 2025/11/09 10:44:00 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_MANAGER_HPP
# define CONNECTION_MANAGER_HPP

# include "all.hpp"

/*==============================================================*/
/*                       ConnectionManager                      */
/*==============================================================*/
/* Description: Multiplexes client connections using poll() and orchestrates request handling. */

class ConnectionManager
{
	public:
		/*===========================*/
		/*  CONSTRUCTORS/DESTRUCTORS */
		/*===========================*/
		ConnectionManager(const std::vector<ServerSocket*>& servers);	// Initialize poll set
		~ConnectionManager(void);										// Clean up owned connections

		/*===========================*/
		/*       PUBLIC METHODS      */
		/*===========================*/
		void	run(void);                                                                                    	// Enter the main polling loop
		void	closeAllConnections(void);																		// Exit gracefully

	private:
		/*===========================*/
		/*        ATTRIBUTES         */
		/*===========================*/
		std::vector<struct pollfd>          _poll_fds;        // Poll descriptors for server and client sockets
		std::vector<ServerSocket*>          _servers;         // Managed listening sockets
		std::map<int, ClientConnection*>    _clients;         // Active client connections indexed by fd
};

#endif
