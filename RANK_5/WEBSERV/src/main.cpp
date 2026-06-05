/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 18:45:28 by jguillem          #+#    #+#             */
/*   Updated: 2025/11/18 12:57:38 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/all.hpp"

volatile std::sig_atomic_t	g_running = 1;

/*
 * Function: handle_sigint
 * Role: Request a graceful shutdown by clearing the global run flag.
 *
 * Parameters:
 *   signum - SIGINT signal value (unused).
 */
void	handle_sigint(int)
{
	// Flip the global flag so the poll loop exits cleanly.
	g_running = 0;
}

/*==============================================================*/
/*                   APPLICATION ENTRY POINT                    */
/*==============================================================*/

int main(int argc, char **argv) {
	std::string configPath = (argc >= 2) ? argv[1] : "conf/default.conf";	// Use provided config file or fallback.

	ConfigParser parser;
	if (!parser.loadFile(configPath) || !parser.parse()) {		// Load+parse config; fail on any error.
		std::cerr << "Configuration error.\n";
		return 1;
	}
	std::vector<ServerConfig>& servers = parser.getServers();	// Retrieve all parsed server blocks.
	if (servers.empty()) {										// Cannot run without at least one server.
		std::cerr << "No valid server blocks found.\n";
		return 1;
	}

	std::vector<ServerSocket*> sockets;		// Store listening sockets.
	std::set<std::string> bound;			// Track host:port pairs already bound.
	sockets.reserve(servers.size());

	for (size_t i = 0; i < servers.size(); ++i) {
		std::ostringstream key;
		key << servers[i].host << ":" << servers[i].port;	// Build a unique identifier per listener.

		if (bound.count(key.str())) {	// Prevent duplicate listeners on same port.
			std::cout << "Skipping duplicate listener on " << key.str() << std::endl;
			continue;
		}
		bound.insert(key.str());

		ServerSocket *sock = new ServerSocket(servers[i]);		// Create socket using server config.
		sock->bindAndListen();									// Bind and enter listening mode.

		std::cout << "Listening on port " << servers[i].port	// Informative startup log.
		          << " root=" << servers[i].root << std::endl;
		sockets.push_back(sock);								// Store for ConnectionManager.
	}

	std::signal(SIGINT, handle_sigint);		// Register Ctrl+C handler.

	ConnectionManager manager(sockets);		// Manage events using poll().
	manager.run();							// Enter main server loop.

	for (size_t i = 0; i < sockets.size(); ++i)	// Cleanup allocated sockets.
		delete sockets[i];

	return 0;
}
