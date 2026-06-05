/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:08:41 by alex              #+#    #+#             */
/*   Updated: 2025/11/16 18:11:32 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALL_HPP
# define ALL_HPP

/*===========================*/
/*    STANDARD LIBRARIES     */
/*===========================*/
# include <algorithm>
# include <cerrno>
# include <cctype>
# include <cstddef>
# include <cstdlib>
# include <cstring>
# include <ctime>
# include <climits>
# include <csignal>
# include <cstdio>
# include <fstream>
# include <iostream>
# include <map>
# include <set>
# include <sstream>
# include <string>
# include <vector>

/*===========================*/
/*      SYSTEM HEADERS       */
/*===========================*/
# include <dirent.h>
# include <fcntl.h>
# include <netinet/in.h>
# include <poll.h>
# include <signal.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/*===========================*/
/*       PROJECT HEADERS     */
/*===========================*/
# include "config/Config.hpp"
# include "config/ConfigParser.hpp"
# include "router/Router.hpp"
# include "http/HttpRequest.hpp"
# include "http/HttpResponse.hpp"
# include "cgi/CGIHandler.hpp"
# include "connect/ServerSocket.hpp"
# include "connect/ClientConnection.hpp"
# include "connect/ConnectionManager.hpp"

#endif
