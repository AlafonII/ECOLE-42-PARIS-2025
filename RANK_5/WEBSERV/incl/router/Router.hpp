/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 13:53:03 by alex              #+#    #+#             */
/*   Updated: 2025/11/14 19:01:04 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTER_HPP
# define ROUTER_HPP

# include "all.hpp"

/*==============================================================*/
/*                               Router                         */
/*==============================================================*/
/* Description: Resolves URIs to filesystem paths and selects error pages. */

class Router
{
	public:
		/*===========================*/
		/*       PUBLIC METHODS      */
		/*===========================*/
		static std::string				resolvePath(std::string& uri, const ServerConfig& cfg); // Map URI to local filesystem path
		static std::string				resolveError(int code, const ServerConfig& cfg);              // Resolve custom error page path
		static const RouteConfig*		matchRoute(const ServerConfig& cfg, const std::string& uri);   // Retrieve best route for URI
};

#endif
