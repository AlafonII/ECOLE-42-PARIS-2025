/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validity.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:32:37 by jguillem          #+#    #+#             */
/*   Updated: 2025/04/18 10:56:37 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* no_exist routine : print error message when there is no file or command
*/
static void	no_exist(char *cmde)
{
	if (*cmde == '.' || *cmde == '/')
		ft_putstr_fd(NO_EXIST, 2);
	else
		ft_putstr_fd(UNKNOWN_CMD, 2);
	exit(127);
}

/*
* exist routine : print error message when the file exists but cannot be exec
*/
static void	exist(char *cmde)
{
	struct stat	st;

	if (!stat(cmde, &st) && S_ISDIR(st.st_mode)
		&& (*cmde == '/' || *cmde == '.'))
	{
		ft_putstr_fd(IS_DIR, 2);
		exit(126);
	}
	else if (access(cmde, X_OK) && !access(cmde, F_OK))
	{
		ft_putstr_fd(FORBIDDEN, 2);
		exit(126);
	}
	else
	{
		ft_putstr_fd(UNKNOWN_CMD, 2);
		exit(127);
	}
}

/*
* check_command_validity
*	- check the status of the command
*	- if necessary exit with corresponding exit value 
*	- else do nothing
*/
void	check_command_validity(char *cmde)
{
	ft_putstr_fd("-minishell: ", 2);
	ft_putstr_fd(cmde, 2);
	if (!ft_strchr(cmde, '/'))
	{
		ft_putstr_fd(UNKNOWN_CMD, 2);
		exit(127);
	}
	if (access(cmde, F_OK))
		no_exist(cmde);
	else
		exist(cmde);
}
