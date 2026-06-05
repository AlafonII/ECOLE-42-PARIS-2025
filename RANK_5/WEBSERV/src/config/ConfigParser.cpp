#include "all.hpp"

/*==============================================================*/
/*                        CONSTRUCTORS                          */
/*==============================================================*/

/*
 * Function: ConfigParser::ConfigParser
 * Role: Initialize an empty parser whose content is supplied later.
 */
ConfigParser::ConfigParser(void) {
	// Nothing to initialize until a configuration file is loaded.
}


/*==============================================================*/
/*                       PRIVATE HELPERS                        */
/*==============================================================*/

/*
 * Function: ConfigParser::trim
 * Role: Remove leading and trailing whitespace characters from a string.
 *
 * Parameters:
 *   s - Input string that may contain surrounding whitespace.
 *
 * Returns:
 *   std::string - Substring containing only the trimmed content.
 */
std::string ConfigParser::trim(const std::string& s) {
	// Identify the first and last non-whitespace characters.
	size_t b = s.find_first_not_of(" \t\r\n");
	size_t e = s.find_last_not_of(" \t\r\n");
	if (b == std::string::npos) return "";
	return s.substr(b, e - b + 1);
}


/*
 * Function: ConfigParser::stripValue
 * Role: Clean directive strings by removing whitespace and trailing separators.
 *
 * Parameters:
 *   s - Raw directive text from the configuration file.
 *
 * Returns:
 *   std::string - Normalized directive value.
 */
std::string ConfigParser::stripValue(const std::string& s) {
	std::string out = trim(s);
	while (!out.empty() && (out[out.size() - 1] == ';' || out[out.size() - 1] == ',')) {
		// Drop trailing separators so parsing logic can focus on the value.
		out.erase(out.size() - 1);
		out = trim(out);
	}
	return out;
}


/*==============================================================*/
/*                   CONFIG BLOCK PARSING                       */
/*==============================================================*/

/*
 * Function: ConfigParser::parseServerBlock
 * Role: Interpret directives inside a server block and create a ServerConfig.
 *
 * Parameters:
 *   block - Raw text belonging to a single server block.
 */
void ConfigParser::parseServerBlock(const std::string& block) {
	std::istringstream ss(block);
	std::string raw;
	ServerConfig cfg;
	// Apply default values so omitted directives inherit safe defaults.
	cfg.port = 8080;
	cfg.host = "0.0.0.0";
	cfg.root = "www";
	cfg.clientMaxBodySize = 1000000;
	while (std::getline(ss, raw)) {
		std::string line = stripValue(raw);
		if (line.empty())
			continue;
		if (line.compare(0, 6, "listen") == 0) {
			std::string value = stripValue(line.substr(6));
			if (!value.empty())
				cfg.port = std::atoi(value.c_str());
		}
		else if (line.compare(0, 4, "host") == 0) {
			cfg.host = stripValue(line.substr(4));
		}
		else if (line.compare(0, 4, "root") == 0) {
			std::string value = stripValue(line.substr(4));
			if (!value.empty())
				cfg.root = value;
		}
		else if (line.compare(0, 20, "client_max_body_size") == 0) {
			std::string value = stripValue(line.substr(20));
			if (!value.empty())
				cfg.clientMaxBodySize = static_cast<size_t>(std::strtoul(value.c_str(), 0, 10));
		}
		else if (line.compare(0, 10, "error_page") == 0) {
			std::istringstream es(stripValue(line.substr(10)));
			int code;
			std::string path;
			if (es >> code >> path)
				cfg.errorPages[code] = path;
		}
		else if (line.compare(0, 8, "location") == 0) {
			RouteConfig route;
			// Reset per-route defaults before merging overrides.
			route.autoindex = false;
			route.allowedMethods.clear();
			route.root.clear();
			route.redirect_status = 0;
			route.redirect_url.clear();
			route.index.clear();
			route.cgiExtension.clear();
			route.cgiPath.clear();
			route.uploadDir.clear();
			route.maxBodySize = 0;
			route.hasMaxBodySize = false;
			route.alias.clear();

			std::string remainder = line.substr(8);
			route.path = stripValue(remainder.substr(0, remainder.find('{')));
			if (route.path.empty())
				route.path = "/";

			int braceDepth = (raw.find('{') != std::string::npos) ? 1 : 0;
			std::string routeBody;

			// Capture any content that trails the opening brace on the same line.
			size_t bracePos = raw.find('{');
			if (bracePos != std::string::npos) {
				std::string afterBrace = raw.substr(bracePos + 1);
				if (!afterBrace.empty()) {
					routeBody += afterBrace;
					routeBody += "\n";
				}
			}

			while (braceDepth > 0 && std::getline(ss, raw)) {
				// Track nested braces so we stop exactly at the matching closing brace.
				if (raw.find('{') != std::string::npos)
					braceDepth++;
				size_t closePos = raw.find('}');
				if (closePos != std::string::npos) {
					std::string before = raw.substr(0, closePos);
					if (!before.empty()) {
						routeBody += before;
						routeBody += "\n";
					}
					braceDepth--;
					std::string trailing = raw.substr(closePos + 1);
					if (!trailing.empty()) {
						routeBody += trailing;
						routeBody += "\n";
					}
					if (braceDepth <= 0)
						break;
				}
				else {
					routeBody += raw;
					routeBody += "\n";
				}
			}
			parseRouteBlock(cfg, route, routeBody);
		}
	}
	_servers.push_back(cfg);
}


/*
 * Function: ConfigParser::parseRouteBlock
 * Role: Parse directives inside a location block and append the route to the server.
 *
 * Parameters:
 *   srv - Server configuration receiving the route.
 *   route - Route object to populate.
 *   block - Raw text of the location block.
 */
void ConfigParser::parseRouteBlock(ServerConfig& srv, RouteConfig& route, const std::string& block) {
	std::istringstream ss(block);
	std::string line;
	while (std::getline(ss, line)) {
		line = stripValue(line);
		if (line.empty()) continue;
		// Apply directive-specific parsing and store it on the route object.
		if (line.find("path", 0) == 0)
			route.path = trim(line.substr(4));
		else if (line.find("root", 0) == 0)
			route.root = stripValue(line.substr(4));
		else if (line.find("index", 0) == 0)
			route.index = stripValue(line.substr(5));
		else if (line.find("methods", 0) == 0) {
			std::istringstream ms(stripValue(line.substr(7)));
			std::string m;
			while (ms >> m) route.allowedMethods.push_back(m);
		}
		else if (line.find("autoindex", 0) == 0) {
			std::string value = stripValue(line.substr(9));
			route.autoindex = (value == "on");
		}
		else if (line.find("redirect", 0) == 0)
		{
			std::string raw = stripValue(line.substr(8));
			if (!raw.empty() && raw[0] == ':')
				raw = trim(raw.substr(1));
			std::istringstream rs(raw);
			std::string statusToken;
			rs >> statusToken;
			std::string urlToken;
			std::getline(rs, urlToken);
			urlToken = trim(urlToken);

			bool validStatus = !statusToken.empty() &&
				statusToken.find_first_not_of("0123456789") == std::string::npos;
			if (!validStatus || urlToken.empty())
			{
				std::cerr << "Invalid redirect directive inside location "
				          << route.path << std::endl;
			}
			else
			{
				route.redirect_status = std::atoi(statusToken.c_str());
				route.redirect_url = urlToken;
			}
		}
		else if (line.find("alias", 0) == 0)
			route.alias = stripValue(line.substr(5));
		else if (line.find("cgi_extension", 0) == 0)
			route.cgiExtension = stripValue(line.substr(13));
		else if (line.find("cgi_path", 0) == 0)
			route.cgiPath = stripValue(line.substr(8));
	else if (line.find("upload_dir", 0) == 0)
		route.uploadDir = stripValue(line.substr(10));
	else if (line.find("client_max_body_size", 0) == 0)
	{
		std::string value = stripValue(line.substr(20));
			if (!value.empty())
			{
				route.maxBodySize = static_cast<size_t>(std::strtoul(value.c_str(), 0, 10));
				route.hasMaxBodySize = true;
			}
		}
	}
	srv.routes.push_back(route);
}


/*==============================================================*/
/*                        CONFIG LOADING                        */
/*==============================================================*/

/*
 * Function: ConfigParser::loadFile
 * Role: Read the configuration file into the parser's internal string buffer.
 *
 * Parameters:
 *   path - Filesystem path to the configuration file.
 *
 * Returns:
 *   bool - true if the file was read successfully, false otherwise.
 */
bool ConfigParser::loadFile(const std::string& path) {
	std::ifstream f(path.c_str());
	if (!f.is_open()) {
		std::cerr << "Cannot open config file: " << path << std::endl;
		return false;
	}
	std::ostringstream ss;
	// Stream entire file into a string buffer for later parsing.
	ss << f.rdbuf();
	this->_content = ss.str();
	return true;
}


/*==============================================================*/
/*                        CONFIG PARSING                        */
/*==============================================================*/

/*
 * Function: ConfigParser::parse
 * Role: Extract each server block from the buffered configuration text.
 *
 * Returns:
 *   bool - true if one or more well-formed server blocks were parsed.
 */
bool ConfigParser::parse(void) {
	size_t pos = 0;
	while ((pos = _content.find("server", pos)) != std::string::npos) {
		size_t open = _content.find("{", pos);
		size_t depth = 1;
		size_t i = open + 1;
		while (i < _content.size() && depth > 0) {
			if (_content[i] == '{')
				depth++;
			else if (_content[i] == '}')
				depth--;
			i++;
		}
		if (depth != 0)
			return false; // malformed braces
		size_t close = i - 1;
		if (open == std::string::npos || close == std::string::npos)
			break;
		std::string block = _content.substr(open + 1, close - open - 1);
		parseServerBlock(block);
		// Continue scanning after the current block.
		pos = close + 1;
	}
	return !_servers.empty();
}


/*==============================================================*/
/*                            GETTERS                           */
/*==============================================================*/

/*
 * Function: ConfigParser::getServers
 * Role: Return the vector of parsed server configurations.
 *
 * Returns:
 *   std::vector<ServerConfig>& - Reference to the stored configurations.
 */
std::vector<ServerConfig>& ConfigParser::getServers(void) {
	return _servers;
}
