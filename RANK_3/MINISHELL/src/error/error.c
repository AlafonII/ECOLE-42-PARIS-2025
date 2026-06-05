/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:16:50 by jguillem          #+#    #+#             */
/*   Updated: 2025/04/19 13:29:07 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* free_envp function :
*	- frees a NULL-terminated array of environment variable strings
*	- releases each string and then the array itself
*/
void	free_envp(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

/*
* free_commands function :
*	- frees a NULL-terminated array of command arrays
*	- releases memory for each word, each command, and the overall array
*/
static void	free_commands(char ***commands)
{
	char	**command;
	char	*word;
	int		i;
	int		j;

	i = 0;
	while (commands[i])
	{
		command = commands[i];
		j = 0;
		while (command[j])
		{
			word = command[j];
			free(word);
			j++;
		}
		free(commands[i]);
		i++;
	}
	free (commands);
}

/*
* free_paths function :
*	- frees a NULL-terminated array of path strings
*	- releases memory for each path and the array itself
*/
static void	free_paths(char **paths)
{
	int	i;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

/*
* free_command_line function :
*	- frees all dynamically allocated memory in a t_cmd_line struct
*	- releases command arrays and path arrays
*/
void	free_command_line(t_cmd_line *cmd_line)
{
	if (cmd_line->commands)
	{
		free_commands(cmd_line->commands);
		cmd_line->commands = NULL;
	}
	if (cmd_line->paths)
	{
		free_paths(cmd_line->paths);
		cmd_line->paths = NULL;
	}
}

/*
* error_handle function :
*	- prints a system error message using perror with the given name
*	- exits the program with EXIT_FAILURE
*/
void	error_handle(char *name)
{
	perror(name);
	exit(EXIT_FAILURE);
}
