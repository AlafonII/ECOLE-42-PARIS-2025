/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:08:28 by alex              #+#    #+#             */
/*   Updated: 2025/11/19 01:25:41 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP
# define CGI_HANDLER_HPP

# include <string>
# include <vector>

class HttpRequest;
struct RouteConfig;

/*==============================================================*/
/*                          CGIHandler                          */
/*==============================================================*/
/* Description: Executes CGI programs using a temp file for STDOUT and a pipe for STDIN. */

class CGIHandler
{
	public:
		/*===========================*/
		/*  CONSTRUCTORS/DESTRUCTORS */
		/*===========================*/
		CGIHandler(const std::vector<std::string>& environment,
				const std::vector<std::string>& arguments,
				const std::string& body,
				const std::string& scriptPath);			// Store execution context
		~CGIHandler(void);								// Default destructor

		/*===========================*/
		/*        PUBLIC METHODS     */
		/*===========================*/
		std::string						run(size_t maxBodySize);									// Execute CGI and return STDOUT as string
		static bool						isCgiRequest(const std::string& path,
													const RouteConfig* route);		// Check extension against route
		static std::vector<std::string>	buildEnvironment(HttpRequest& request,
													const std::string& filePath);	// Build CGI env variables
		static std::vector<std::string>	buildArguments(const RouteConfig* route,
													const std::string& filePath);	// Build argv array elements

	private:
		CGIHandler(void);
		CGIHandler(const CGIHandler& other);
		CGIHandler&	operator=(const CGIHandler& other);

		/*===========================*/
		/*        PRIVATE METHODS    */
		/*===========================*/
		std::vector<char*>			buildEnvpArray(void);	// Convert stored env strings to char*[]
		std::vector<char*>			buildArgvArray(void);	// Convert stored argv strings to char*[]

		/*===========================*/
		/*         ATTRIBUTES        */
		/*===========================*/
		std::vector<std::string>	_environment;	// Stored environment strings
		std::vector<std::string>	_arguments;		// Stored argv strings
		std::string					_body;			// Request body to forward to CGI
		std::string					_scriptPath;	// CGI script path on disk
};

#endif
