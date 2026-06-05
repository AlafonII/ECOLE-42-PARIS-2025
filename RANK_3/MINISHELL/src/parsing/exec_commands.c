/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:34:35 by alex              #+#    #+#             */
/*   Updated: 2025/04/19 21:34:38 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* handle_external_exec function :
*	- resolves and executes an external command
*	- exits cleanly if the command is empty or resolution fails
*	- prints an error and exits if execve fails
*/
static void	handle_external_exec(t_cmd_line *cmd_line, int i)
{
	char	*cmd;
	char	*path;

	cmd = cmd_line->commands[i][0];
	if (!cmd || !ft_strlen(cmd))
	{
		free_command_line(cmd_line);
		free_envp(cmd_line->envp);
		exit(EXIT_SUCCESS);
	}
	path = resolve_or_exit(cmd_line, i);
	execve(path, cmd_line->commands[i], cmd_line->envp);
	perror("execve");
	free(path);
	free_command_line(cmd_line);
	free_envp(cmd_line->envp);
	exit(EXIT_FAILURE);
}

/*
* handle_builtin_exec function :
*	- executes a builtin command in a child process context
*	- frees command line resources and environment variables
*	- exits successfully after execution
*/
static void	handle_builtin_exec(char **args, t_cmd_line *cmd_line)
{
	except_built_in(args, cmd_line);
	free_command_line(cmd_line);
	free_envp(cmd_line->envp);
	exit(EXIT_SUCCESS);
}

/*
* execute_command function :
*	- determines if the command is a parent-only builtin or external
*	- executes builtins directly or delegates to external execution handler
*	- returns 0 (function never actually returns if execution is successful)
*/
int	execute_command(t_cmd_line *cmd_line, int i)
{
	char	*cmd;

	cmd = cmd_line->commands[i][0];
	if (cmd && is_parent_builtin(cmd))
		handle_builtin_exec(cmd_line->commands[i], cmd_line);
	if (cmd && (!ft_strcmp(cmd, "echo")
			|| !ft_strcmp(cmd, "env")
			|| !ft_strcmp(cmd, "pwd")))
		handle_builtin_exec(cmd_line->commands[i], cmd_line);
	handle_external_exec(cmd_line, i);
	return (0);
}
