/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 13:11:58 by jguillem          #+#    #+#             */
/*   Updated: 2025/11/21 13:19:42 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

/*==============================================================*/
/*                        CONSTRUCTORS                          */
/*==============================================================*/

/*
 * Function: ConnectionManager::ConnectionManager
 * Role: Initialize the poll set using the provided listening sockets.
 *
 * Parameters:
 *   servers - Array of server sockets that should be monitored.
 */
ConnectionManager::ConnectionManager(const std::vector<ServerSocket*>& servers)
	: _servers(servers)
{
	for (size_t i = 0; i < servers.size(); ++i)
	{
		// Track listening sockets so new clients can be accepted.
		struct pollfd pfd = { servers[i]->getFd(), POLLIN, 0 };
		_poll_fds.push_back(pfd);
	}
	std::cout << "[ConnectionManager] Monitoring " << servers.size()
	          << " listening sockets." << std::endl;
}

/*==============================================================*/
/*                        DESTRUCTORS                           */
/*==============================================================*/

/*
 * Function: ConnectionManager::~ConnectionManager
 * Role: Free dynamically allocated ClientConnection objects.
 */
ConnectionManager::~ConnectionManager(void)
{
	for (std::map<int, ClientConnection*>::iterator it = _clients.begin();
		 it != _clients.end(); ++it)
		delete it->second;
}

/*==============================================================*/
/*                POLL LOOP / EVENT HANDLING                    */
/*==============================================================*/

/*
 * Function: ConnectionManager::run
 * Role: Enter the main poll loop that accepts clients and processes I/O.
 */
void	ConnectionManager::run(void)
{
	extern volatile std::sig_atomic_t	g_running;

	while (g_running)
	{
		if (_poll_fds.empty())
			break;

		// Poll sockets with a timeout to remain responsive to signals.
		int poll_res = poll(&_poll_fds[0], (nfds_t)_poll_fds.size(), 500);
		if (poll_res < 0)
		{
			std::cerr << strerror(errno) << std::endl;
			break;
		}
		if (poll_res == 0)
			continue;

		for (size_t i = 0; i < _poll_fds.size(); ++i)
		{
			int fd = _poll_fds[i].fd;
			short revents = _poll_fds[i].revents;

			if (revents & (POLLERR | POLLHUP | POLLNVAL))
			{
				// Tear down errored client connections immediately.
				std::map<int, ClientConnection*>::iterator it = _clients.find(fd);
				if (it != _clients.end())
				{
					it->second->closeConnection();
					delete it->second;
					_clients.erase(it);
					_poll_fds.erase(_poll_fds.begin() + i);
					i--;
				}
				continue;
			}

			// --- Incoming connections ---
			if (revents & POLLIN)
			{
				// Is this one of the listening sockets?
				bool is_server_fd = false;
				for (size_t s = 0; s < _servers.size(); ++s)
				{
					if (fd == _servers[s]->getFd())
					{
						is_server_fd = true;
						int client_fd = _servers[s]->acceptConnection();
						if (client_fd >= 0)
						{
							ClientConnection* new_client =
								new ClientConnection(client_fd, _servers[s]->getConfig());
							_clients[client_fd] = new_client;

							struct pollfd cli;
							cli.fd = client_fd;
							cli.events = POLLIN | POLLOUT;
							cli.revents = 0;
							_poll_fds.push_back(cli);
							std::cout << "[+] Client connected (fd " << client_fd
							          << ") on port " << _servers[s]->getConfig().port << std::endl;
						}
						break;
					}
				}

				if (!is_server_fd)
				{
					std::map<int, ClientConnection*>::iterator it = _clients.find(fd);
					if (it == _clients.end())
						continue;

					ClientConnection* client = it->second;
					if (!client->readRequest())
					{
						// If the request is incomplete or the client closed, manage lifecycle.
						if (client->isClosed())
						{
							delete client;
							_clients.erase(it);
							_poll_fds.erase(_poll_fds.begin() + i);
							i--;
						}
						continue;
					}
					std::string resp = client->etablishResponse();
					// Store the generated response so POLLOUT can flush it.
					client->queueResponse(resp);
				}
			}

			if (revents & POLLOUT)
			{
				std::map<int, ClientConnection*>::iterator it = _clients.find(fd);
				if (it == _clients.end())
					continue;

				ClientConnection* client = it->second;
				if (client->hasResponseReady())
				{
					// Send as much of the queued response as the socket accepts.

					if (client->sendResponse() || client->shouldClose())
					{
						client->closeConnection();
						delete client;
						_clients.erase(it);
						_poll_fds.erase(_poll_fds.begin() + i);
						i--;
					}
					else
						client->resetRequest();
				}
			}
		}
	}
	std::cout << std::endl << "[!] Shutting down gracefully..." << std::endl;
	this->closeAllConnections();
}

/*
 * Function: ConnectionManager::closeAllConnections
 * Role: Close and delete every tracked client connection.
 */
void	ConnectionManager::closeAllConnections(void)
{
	for (std::map<int, ClientConnection*>::iterator	it = this->_clients.begin();
			it != this->_clients.end(); it++)
	{
		// Ensure socket descriptors are closed before deleting the object
		it->second->closeConnection();
		delete it->second;
	}
	this->_clients.clear();
	this->_poll_fds.clear();
}
