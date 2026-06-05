/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   except.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 11:00:29 by jguillem          #+#    #+#             */
/*   Updated: 2025/04/19 17:56:51 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** except_built_in:
**  Checks if the first token of the command is a built-in and, if so,
**  executes it. For export, we pass the environment pointer (inside t_cmd_line)
**  so that ft_export can update it.
*/
int	except_built_in(char **command, t_cmd_line *cmd_line)
{
	if (!command || !*command)
		return (0);
	if (!ft_strcmp(*command, "export"))
		return (ft_export(command, &cmd_line->envp));
	if (!ft_strcmp(*command, "env"))
		return (ft_env(command, cmd_line->envp), 1);
	if (!ft_strcmp(*command, "echo"))
		return (ft_echo(command), 1);
	if (!ft_strcmp(*command, "exit"))
		return (ft_exit(cmd_line));
	if (!ft_strcmp(*command, "unset"))
		return (ft_unset(command, &cmd_line->envp));
	if (!ft_strcmp(*command, "cd"))
		return (ft_cd(command, &cmd_line->envp));
	if (!ft_strcmp(*command, "pwd"))
		return (ft_pwd(command));
	return (0);
}

/*
* is_parent_builtin function :
*	- checks if a command is a builtin that must be run in the parent process
*	- returns 1 for export, unset, exit, or cd; 0 otherwise
*/
int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "cd"))
		return (1);
	return (0);
}
