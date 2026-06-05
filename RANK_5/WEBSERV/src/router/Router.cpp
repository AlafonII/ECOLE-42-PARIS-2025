/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 13:53:03 by alex              #+#    #+#             */
/*   Updated: 2025/11/21 10:48:38 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

/*==============================================================*/
/*                       STATIC HELPERS                         */
/*==============================================================*/

/*
 * Function: normalizePath
 * Role: Clean a URI by resolving '.' and '..' segments and ensuring a leading slash.
 *
 * Parameters:
 *   in - Original URI string to normalize.
 *
 * Returns:
 *   std::string - Normalized path suitable for route matching.
 */
static std::string normalizePath(const std::string& in) {
	std::vector<std::string> parts;		// Stores normalized path components
	std::string elem, path = in;		// 'elem' builds each path segment

	if (path.empty() || path[0] != '/') path = "/" + path;	// Ensure path always starts with '/'

	for (size_t i = 0; i <= path.size(); ++i) {
		if (i == path.size() || path[i] == '/') {
			if (elem == ".." && !parts.empty())
				parts.pop_back();
			else if (!elem.empty() && elem != ".")
				parts.push_back(elem);	// Add normal segment (skip "" and ".")
			elem.clear();				// Reset builder
		} else
			elem.push_back(path[i]);	// Accumulate characters for the segment
	}

	std::string out = "/";

	for (size_t k = 0; k < parts.size(); ++k) {
		out += parts[k];
		if (k + 1 < parts.size())
			out += "/";		// Add separator between segments
	}

	return out;
}

/*
 * Function: selectRoute
 * Role: Choose the best matching route for the normalized URI.
 *
 * Parameters:
 *   cfg - Server configuration containing routes.
 *   normUri - Normalized URI to match against.
 *
 * Returns:
 *   const RouteConfig* - Pointer to the best-matching route or NULL.
 */
static const RouteConfig*	selectRoute(const ServerConfig& cfg, const std::string& normUri) {
	const RouteConfig* best = 0;	// Best-matching route so far
	size_t bestLen = 0;				// Length of longest matching prefix

	for (size_t i = 0; i < cfg.routes.size(); ++i) {
		const RouteConfig& route = cfg.routes[i];
		if (route.path.empty())
			continue;

		std::string path = route.path;
		if (path[0] != '/')
			path.insert(path.begin(), '/');

		bool matches = false;

		if (path == "/")
			matches = true;
		else if (
			normUri.compare(0, path.size(), path) == 0 &&
			(normUri.size() == path.size() || normUri[path.size()] == '/')
		)
			matches = true;

		if (matches && path.size() >= bestLen) {
			best = &route;
			bestLen = path.size();
		}
	}

	return best;
}

/*
 * Function: Router::matchRoute
 * Role: Expose the route selection logic for external callers.
 *
 * Parameters:
 *   cfg - Server configuration to search within.
 *   uri - Incoming request URI.
 *
 * Returns:
 *   const RouteConfig* - Matching route or NULL if no route matches.
 */
const RouteConfig* Router::matchRoute(const ServerConfig& cfg, const std::string& uri) {
	std::string norm = normalizePath(uri);
	return selectRoute(cfg, norm);
}

/*==============================================================*/
/*                        HTTP ROUTING                          */
/*==============================================================*/

/*
 * Function: Router::resolvePath
 * Role: Convert a URI into a filesystem path according to routing rules.
 *
 * Parameters:
 *   uri - Request URI supplied by the client.
 *   cfg - Server configuration containing root and route definitions.
 *
 * Returns:
 *   std::string - Filesystem path corresponding to the request.
 */
std::string Router::resolvePath(std::string& uri, const ServerConfig& cfg) {
	const RouteConfig* route = Router::matchRoute(cfg, uri);	// Select the best matching route for this URI

	// Choose route-level root if present, otherwise server-level root
	std::string root = (route && !route->root.empty()) ? route->root : cfg.root;
	if (root.empty())
		root = "www";	// Default fallback root folder

	// Remove trailing slash to avoid duplicated separators later
	if (!root.empty() && root[root.size() - 1] == '/')
		root.erase(root.size() - 1);

	// If the route defines an alias, replace the matched route prefix with the alias
	if (route && route->alias.length())
		uri.replace(0, route->path.length(), route->alias);

	// Remove the leading '/' from the URI so we can append it cleanly to the root
	std::string cleanUri = uri;
	if (!cleanUri.empty() && cleanUri[0] == '/')
		cleanUri.erase(0, 1);

	// Build final filesystem path: root + "/" + cleanUri
	std::string fullPath = root;
	if (!cleanUri.empty()) {
		if (!fullPath.empty())
			fullPath += "/";
		fullPath += cleanUri;
	}
	else if (fullPath.empty())
		fullPath = "/";			// Edge case: empty root + empty URI

	// ---- DIRECTORY HANDLING ----
	struct stat st;
	if (stat(fullPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
		if (!route || !route->autoindex) {	// Only append index if autoindex is OFF.
			std::string indexName = (route && !route->index.empty())
				? route->index : "index.html";	// Route-level index or default "index.html"
			if (!fullPath.empty() && fullPath[fullPath.size() - 1] != '/')
				fullPath += "/";
			fullPath += indexName;
		}
	}
	// URI ends with "/" but fullPath isn't an actual directory
	else if (!uri.empty() && uri[uri.size() - 1] == '/') {
		if (!route || !route->autoindex) {
			std::string indexName = (route && !route->index.empty())
				? route->index : "index.html";
			if (!fullPath.empty() && fullPath[fullPath.size() - 1] != '/')
				fullPath += "/";
			fullPath += indexName;
		}
	}

	return fullPath;
}

/*==============================================================*/
/*                        ERROR HANDLING                        */
/*==============================================================*/

/*
 * Function: Router::resolveError
 * Role: Look up the configured error page for a given status code.
 *
 * Parameters:
 *   code - HTTP status code requiring an error page.
 *   cfg - Server configuration containing custom error mappings.
 *
 * Returns:
 *   std::string - Path to the error page, defaulting to root/404.html.
 */
std::string Router::resolveError(int code, const ServerConfig& cfg) {
	std::map<int,std::string>::const_iterator it = cfg.errorPages.find(code);	// Check if this error code has a custom error page
	if (it != cfg.errorPages.end()) return it->second;							// If found, return the custom page path
	return (cfg.root + "/404.html");											// Fallback: use server root + default 404.html
}
