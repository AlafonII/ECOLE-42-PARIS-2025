/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:34:41 by alex              #+#    #+#             */
/*   Updated: 2025/04/19 21:34:43 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* resolve_local_command function :
*	- checks if a local command exists and is executable
*	- handles errors for non-existent files, directories
*	- returns a duplicated string of the command path
*/
static char	*resolve_local_command(t_cmd_line *cmd_line, int i)
{
	char			*cmd;
	char			*path;
	struct stat		st;

	cmd = cmd_line->commands[i][0];
	if (access(cmd, F_OK) != 0)
	{
		ft_putstr_fd("-minishell: ", 2);
		perror(cmd);
		exit(127);
	}
	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("-minishell: ", 2);
		ft_putstr_fd(IS_DIR, 2);
		exit(126);
	}
	if (access(cmd, X_OK) != 0)
	{
		ft_putstr_fd("-minishell: ", 2);
		perror(cmd);
		exit(126);
	}
	path = ft_strdup(cmd);
	return (path);
}

/*
* resolve_command_path function :
*	- determines how to resolve a command's executable path
*	- calls resolve_local_command if the command includes a '/'
*	- otherwise, searches for the command in the system PATH using find_path
*/
char	*resolve_command_path(t_cmd_line *cmd_line, int i)
{
	char	*cmd;

	cmd = cmd_line->commands[i][0];
	if (ft_strchr(cmd, '/'))
		return (resolve_local_command(cmd_line, i));
	else
		return (find_path(cmd, cmd_line->paths));
}

/*
* resolve_or_exit function :
*	- resolves the full path of a command or prints an error and exits
*	- used when execution cannot continue without a valid command path
*	- returns the resolved path on success
*/
char	*resolve_or_exit(t_cmd_line *cmd_line, int i)
{
	char	*cmd;
	char	*path;

	cmd = cmd_line->commands[i][0];
	path = resolve_command_path(cmd_line, i);
	if (!path)
	{
		ft_putstr_fd("-minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(UNKNOWN_CMD, 2);
		free_command_line(cmd_line);
		free_envp(cmd_line->envp);
		exit(127);
	}
	return (path);
}
