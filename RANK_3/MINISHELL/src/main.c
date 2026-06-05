/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:56:21 by jguillem          #+#    #+#             */
/*   Updated: 2025/04/21 20:42:23 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* copy_envp function :
*	- duplicates environment variables from src to dst
*	- returns -1 on success, or the index of the failed duplication
*/
static int	copy_envp(char **src, char **dst)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dst[i] = ft_strdup(src[i]);
		if (!dst[i])
			return (i);
		i++;
	}
	return (-1);
}

/*
* duplicate_env function :
*	- creates a deep copy of the environment variable array
*	- handles empty or NULL input
*	- returns the duplicated array or NULL on failure
*/
char	**duplicate_env(char **envp)
{
	char	**dup;
	int		count;
	int		err;

	if (!envp)
	{
		dup = malloc(sizeof(char *) * 1);
		if (!dup)
			return (NULL);
		return (dup[0] = NULL, dup);
	}
	count = 0;
	while (envp[count])
		count++;
	dup = malloc(sizeof(char *) * (count + 1));
	if (!dup)
		return (NULL);
	err = copy_envp(envp, dup);
	if (err >= 0)
	{
		while (err--)
			free(dup[err]);
		return (free(dup), NULL);
	}
	return (dup[count] = NULL, dup);
}

/*
* process_input function :
*	- processes a single line of user input
*	- adds non-empty, non-whitespace-only input to history
*	- prints a warning if the line ends with a pipe, otherwise processes it
*/
static void	process_input(char *txt, t_cmd_line *cmd_line)
{
	size_t	len;

	len = ft_strlen(txt);
	if (len == 0)
		return ;
	if (!ft_isonlyspace(txt))
		add_history(txt);
	process_cmd_line(txt, &cmd_line->envp);
}

/*
* shell_loop function :
*	- main interactive loop of the shell
*	- sets up signal handling and reads user input
*	- processes input or exits cleanly on EOF (Ctrl+D)
*/
static void	shell_loop(t_cmd_line *cmd_line)
{
	char	*txt;

	while (1)
	{
		signal(SIGINT, &sighandler);
		signal(SIGQUIT, SIG_IGN);
		txt = readline(get_prompt());
		if (!txt)
		{
			printf("exit\n");
			clear_history();
			free_envp(cmd_line->envp);
			exit(0);
		}
		process_input(txt, cmd_line);
		free(txt);
	}
}

/*
* main function :
*	- initializes the shell environment and enters the main loop
*	- duplicates the system environment and starts the shell_loop
*	- cleans up history and environment before exiting
*/
int	main(int argc, char **argv, char **envp)
{
	t_cmd_line	cmd_line;

	(void)argc;
	(void)argv;
	cmd_line.envp = duplicate_env(envp);
	if (!cmd_line.envp)
		error_handle("malloc error");
	shell_loop(&cmd_line);
	clear_history();
	free_envp(cmd_line.envp);
	return (0);
}
