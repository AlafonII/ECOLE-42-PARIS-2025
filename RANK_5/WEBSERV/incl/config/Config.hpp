/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 18:13:37 by alex              #+#    #+#             */
/*   Updated: 2025/11/14 17:17:35 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "all.hpp"

/*==============================================================*/
/*                           RouteConfig                        */
/*==============================================================*/
/* Description: Represents a single location block within a server. */

struct RouteConfig
{
	bool						autoindex;          // Enables directory listing
	std::string					path;               // Location URI prefix
	std::string					root;               // Filesystem root for this route
	int							redirect_status;    // HTTP status code for redirect (0 = disabled)
	std::string					redirect_url;       // Redirect destination URL
	std::string					index;              // Default index file
	std::string					cgiExtension;       // File extension triggering CGI
	std::string					cgiPath;            // Executable path for CGI handling
	std::string					uploadDir;          // Destination directory for uploads
	std::string					alias;				// Alias of the location
	size_t						maxBodySize;        // Optional per-route body limit
	bool						hasMaxBodySize;     // Indicates if route overrides body limit
	std::vector<std::string>	allowedMethods;     // HTTP verbs allowed on this route
};


/*==============================================================*/
/*                           ServerConfig                       */
/*==============================================================*/
/* Description: Holds configuration for a single server instance. */

struct ServerConfig
{
	int							port;               // Listening port number
	size_t						clientMaxBodySize;  // Maximum accepted body size in bytes
	std::string					host;               // Interface address to bind
	std::string					root;               // Default document root
	std::map<int, std::string>	errorPages;         // Custom error pages per status code
	std::vector<RouteConfig>	routes;             // Routes (location blocks) served
};

#endif
