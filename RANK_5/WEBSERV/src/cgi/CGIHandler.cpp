/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:08:46 by alex              #+#    #+#             */
/*   Updated: 2025/11/19 01:43:20 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

/*==============================================================*/
/*                        CONSTRUCTORS                          */
/*==============================================================*/

/*
 * Function: CGIHandler::CGIHandler
 * Role: Initialize the runner with CGI arguments, environment, body, and script path.
 *
 * Parameters:
 *   environment - List of environment variables formatted as KEY=VALUE.
 *   arguments - argv vector, where arguments[0] is the CGI executable path.
 *   body - Raw HTTP request body to forward on STDIN.
 *   scriptPath - Filesystem path to the CGI script being executed.
 */
CGIHandler::CGIHandler(const std::vector<std::string>& environment,
		const std::vector<std::string>& arguments,
		const std::string& body,
		const std::string& scriptPath)
	: _environment(environment)
	, _arguments(arguments)
	, _body(body)
	, _scriptPath(scriptPath)
{
	return;
}

/*==============================================================*/
/*                        DESTRUCTORS                           */
/*==============================================================*/

/*
 * Function: CGIHandler::~CGIHandler
 * Role: Default destructor; file descriptors are managed per run().
 */
CGIHandler::~CGIHandler(void)
{
	return;
}

/*==============================================================*/
/*                         PUBLIC METHODS                       */
/*==============================================================*/

/*
 * Function: CGIHandler::run
 * Role: Execute the configured CGI program, stream the request body via a pipe, capture output, and strip CGI headers.
 *
 * Returns:
 *   std::string - CGI stdout body (empty on failure).
 */
std::string CGIHandler::run(size_t maxBodySize)
{
	std::string output;

	int         pipeStdin[2];
	int         pipeStdout[2];
	int         pipeStderr[2];
	pid_t       pid;

	if (pipe(pipeStdin) < 0) return output;
	if (pipe(pipeStdout) < 0)
	{
		close(pipeStdin[0]); close(pipeStdin[1]);
		return output;
	}
	if (pipe(pipeStderr) < 0)
	{
		close(pipeStdin[0]); close(pipeStdin[1]);
		close(pipeStdout[0]); close(pipeStdout[1]);
		return output;
	}

	pid = fork();
	/* Child */
	if (pid == 0)
	{
		std::vector<char*> envp = this->buildEnvpArray();
		std::vector<char*> argv = this->buildArgvArray();
		if (argv.empty()) std::exit(1);

		if (dup2(pipeStdin[0], STDIN_FILENO) < 0) std::exit(1);
		if (dup2(pipeStdout[1], STDOUT_FILENO) < 0) std::exit(1);
		if (dup2(pipeStderr[1], STDERR_FILENO) < 0) std::exit(1);

		close(pipeStdin[0]); close(pipeStdin[1]);
		close(pipeStdout[0]); close(pipeStdout[1]);
		close(pipeStderr[0]); close(pipeStderr[1]);

		argv.push_back(NULL);
		envp.push_back(NULL);

		execve(argv[0], &argv[0], &envp[0]);
		std::exit(1);
	}
	/* Fork error */
	else if (pid < 0)
	{
		close(pipeStdin[0]); close(pipeStdin[1]);
		close(pipeStdout[0]); close(pipeStdout[1]);
		close(pipeStderr[0]); close(pipeStderr[1]);
		return output;
	}

	/* Parent */
	close(pipeStdin[0]);
	close(pipeStdout[1]);
	close(pipeStderr[1]);

	const size_t CHUNK = 65536;
	size_t sent = 0;
	char buffer[CHUNK];
	ssize_t bytesRead;
	bool finishedWriting = false;

	size_t totalRead = 0;

	while (!finishedWriting)
	{
		if (sent < _body.size())
		{
			size_t toWrite = std::min(CHUNK, _body.size() - sent);
			ssize_t n = write(pipeStdin[1], _body.c_str() + sent, toWrite);
			if (n <= 0)
				break;

			sent += static_cast<size_t>(n);
		}

		if (sent >= _body.size())
		{
			close(pipeStdin[1]);
			finishedWriting = true;
		}

		bytesRead = read(pipeStdout[0], buffer, sizeof(buffer));
		if (bytesRead > 0)
		{
			output.append(buffer, static_cast<size_t>(bytesRead));
			totalRead += static_cast<size_t>(bytesRead);

			if (totalRead > maxBodySize)
			{
				kill(pid, SIGKILL);
				waitpid(pid, NULL, 0);
				output += "\n[CGI terminated: output limit reached]\n";
				close(pipeStdout[0]);
				close(pipeStderr[0]);
				return output;
			}
		}
	}

	while ((bytesRead = read(pipeStdout[0], buffer, sizeof(buffer))) > 0)
	{
		output.append(buffer, static_cast<size_t>(bytesRead));
		totalRead += static_cast<size_t>(bytesRead);

		if (totalRead > maxBodySize)
		{
			kill(pid, SIGKILL);
			waitpid(pid, NULL, 0);
			output += "\n[CGI terminated: output limit reached]\n";
			close(pipeStdout[0]);
			close(pipeStderr[0]);
			return output;
		}
	}
	close(pipeStdout[0]);

	while ((bytesRead = read(pipeStderr[0], buffer, sizeof(buffer))) > 0)
	{
		output.append(buffer, static_cast<size_t>(bytesRead));
		totalRead += static_cast<size_t>(bytesRead);

		if (totalRead > maxBodySize)
		{
			kill(pid, SIGKILL);
			waitpid(pid, NULL, 0);
			output += "\n[CGI terminated: output limit reached]\n";
			close(pipeStderr[0]);
			return output;
		}
	}
	close(pipeStderr[0]);

	waitpid(pid, NULL, 0);

	size_t headerEnd = output.find("\r\n\r\n");
	size_t delimiterLen = 4;
	if (headerEnd == std::string::npos)
	{
		headerEnd = output.find("\n\n");
		delimiterLen = 2;
	}
	if (headerEnd != std::string::npos)
		output = output.substr(headerEnd + delimiterLen);

	return output;
}

/*
 * Function: CGIHandler::isCgiRequest
 * Role: Determine whether a given path matches the configured CGI extension.
 *
 * Parameters:
 *   path - Target file path derived from the request URI.
 *   route - Route configuration containing CGI metadata.
 *
 * Returns:
 *   bool - true if the extension matches the route CGI extension, otherwise false.
 */
bool	CGIHandler::isCgiRequest(const std::string& path, const RouteConfig* route)
{
	if (route == NULL)
		return (false);

	const std::string&	cgiExt = route->cgiExtension;

	if (!cgiExt.empty())
	{
		size_t	dotPos = path.rfind('.');
		if (dotPos != std::string::npos)
		{
			std::string	ext = path.substr(dotPos);
			if (ext == cgiExt)
				return (true);
		}
	}
	return (false);
}

/*
 * Function: CGIHandler::buildEnvironment
 * Role: Build the set of environment variables required for CGI execution.
 *
 * Parameters:
 *   request - Parsed HTTP request providing headers and properties.
 *   filePath - Filesystem path to the CGI script.
 *
 * Returns:
 *   std::vector<std::string> - Collection of KEY=VALUE strings.
 */
std::vector<std::string>	CGIHandler::buildEnvironment(HttpRequest& request,
		const std::string& filePath)
{
	std::vector<std::string>	envStrings;
	std::ostringstream			lengthStream;
	std::string					hostHeader = request.getHeader("Host");
	std::string					serverName = hostHeader;
	std::string					serverPort = "80";
	size_t						colonPos = hostHeader.find(':');

	if (colonPos != std::string::npos)
	{
		serverName = hostHeader.substr(0, colonPos);
		serverPort = hostHeader.substr(colonPos + 1);
	}

	lengthStream << request.getBody().size();
	std::string	requestUri = request.getPath();
	if (!request.getQuery().empty())
		requestUri += "?" + request.getQuery();

	envStrings.push_back("REQUEST_METHOD=" + request.getMethod());
	envStrings.push_back("SCRIPT_FILENAME=" + filePath);
	envStrings.push_back("PATH_INFO=" + request.getPath());
	envStrings.push_back("QUERY_STRING=" + request.getQuery());
	envStrings.push_back("CONTENT_LENGTH=" + lengthStream.str());
	envStrings.push_back("CONTENT_TYPE=" + request.getHeader("Content-Type"));
	envStrings.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envStrings.push_back("SERVER_NAME=" + serverName);
	envStrings.push_back("SERVER_PORT=" + serverPort);
	envStrings.push_back("REQUEST_URI=" + requestUri);
	envStrings.push_back("REDIRECT_STATUS=200");
	std::map<std::string, std::string>& headers = request.getHeaders();
	for (std::map<std::string, std::string>::iterator it = headers.begin();
			it != headers.end(); ++it)
	{
		std::string key = it->first;
		for (size_t i = 0; i < key.size(); ++i)
		{
			if (key[i] >= 'a' && key[i] <= 'z')
				key[i] = static_cast<char>(key[i] - 'a' + 'A');
			else if (key[i] == '-')
				key[i] = '_';
		}
		envStrings.push_back("HTTP_" + key + "=" + it->second);
	}
	return (envStrings);
}

/*
 * Function: CGIHandler::buildArguments
 * Role: Build the argv list for the CGI process.
 *
 * Parameters:
 *   route - Route configuration pointing to the CGI executable.
 *   filePath - Path to the script being executed (kept for symmetry with the interface).
 *
 * Returns:
 *   std::vector<std::string> - Arguments beginning with the CGI executable path.
 */
std::vector<std::string>	CGIHandler::buildArguments(const RouteConfig* route,
		const std::string& filePath)
{
	(void)filePath;
	std::vector<std::string>	args;

	if (route && !route->cgiPath.empty())
		args.push_back(route->cgiPath);
	return (args);
}

/*==============================================================*/
/*                        PRIVATE METHODS                       */
/*==============================================================*/

/*
 * Function: CGIHandler::buildEnvpArray
 * Role: Convert the stored environment strings into a NULL-terminated char* array.
 *
 * Returns:
 *   std::vector<char*> - Pointers valid as long as _environment strings remain alive.
 */
std::vector<char*>	CGIHandler::buildEnvpArray(void)
{
	std::vector<char*>	envp;

	envp.reserve(this->_environment.size() + 1);
	for (size_t i = 0; i < this->_environment.size(); ++i)
		envp.push_back(const_cast<char*>(this->_environment[i].c_str()));
	envp.push_back(NULL);
	return (envp);
}

/*
 * Function: CGIHandler::buildArgvArray
 * Role: Convert the stored argument strings into a NULL-terminated char* array.
 *
 * Returns:
 *   std::vector<char*> - Pointers valid as long as _arguments strings remain alive.
 */
std::vector<char*>	CGIHandler::buildArgvArray(void)
{
	std::vector<char*>	argv;

	argv.reserve(this->_arguments.size() + 1);
	for (size_t i = 0; i < this->_arguments.size(); ++i)
		argv.push_back(const_cast<char*>(this->_arguments[i].c_str()));
	argv.push_back(NULL);
	return (argv);
}
