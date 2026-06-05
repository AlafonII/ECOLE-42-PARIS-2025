/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 18:17:11 by alex              #+#    #+#             */
/*   Updated: 2025/11/06 01:08:23 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include "all.hpp"

/*==============================================================*/
/*                          ConfigParser                        */
/*==============================================================*/
/* Description: Loads and parses configuration files into ServerConfig objects. */

class ConfigParser
{
	public:
		/*===========================*/
		/*  CONSTRUCTORS/DESTRUCTORS */
		/*===========================*/
		ConfigParser(void);                                   // Default constructor

		/*===========================*/
		/*       PUBLIC METHODS      */
		/*===========================*/
		bool	loadFile(const std::string& path);            // Read configuration file content
		bool	parse(void);                                  // Parse loaded configuration
		std::vector<ServerConfig>&	getServers(void);        // Access parsed server definitions

	private:
		/*===========================*/
		/*      PRIVATE HELPERS      */
		/*===========================*/
		void		parseServerBlock(const std::string& block);                               // Parse a single server block
		void		parseRouteBlock(ServerConfig& srv, RouteConfig& route, const std::string& block); // Parse a location block
		std::string	trim(const std::string& s);                                              // Trim whitespace from both ends
		std::string	stripValue(const std::string& s);                                         // Clean value (remove delimiters)

		/*===========================*/
		/*         ATTRIBUTES        */
		/*===========================*/
		std::string					_content;       // Raw configuration file contents
		std::vector<ServerConfig>	_servers;       // Parsed server configurations
};

#endif
