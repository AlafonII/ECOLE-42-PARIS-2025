/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:14:27 by jguillem          #+#    #+#             */
/*   Updated: 2025/04/19 19:24:30 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* son_pipeline function : manage pipe in the son level
*/
static int	son_pipeline(t_cmd_line *cmd_line, t_cmd_meta *cmd_meta, int i)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd_line->pids)
	{
		free(cmd_line->pids);
		cmd_line->pids = NULL;
	}
	if (cmd_meta->prev_fd != -1)
	{
		if (dup2(cmd_meta->prev_fd, STDIN_FILENO) < 0)
			error_handle("dup2");
		close(cmd_meta->prev_fd);
	}
	if (cmd_line->commands[i + 1] && *(cmd_line->commands[i + 1]))
	{
		if (dup2(cmd_meta->pipefd[1], STDOUT_FILENO) < 0)
			error_handle("dup2");
		close(cmd_meta->pipefd[1]);
		close(cmd_meta->pipefd[0]);
	}
	redirect_analysis(cmd_line, cmd_meta, i);
	expand_tokens(&cmd_line->commands[i], cmd_line->envp);
	return (execute_command(cmd_line, i));
}

/*
* pipeline_iteration routine : process for each command of commands
*/
static int	pipeline_iteration(t_cmd_line *cmd_line,
	t_cmd_meta *cmd_meta, int i)
{
	pid_t		son;

	signal(SIGINT, SIG_IGN);
	if (cmd_line->commands[i + 1])
	{
		if (pipe(cmd_meta->pipefd) == -1)
			error_handle("pipe");
	}
	son = fork();
	if (son == -1)
		error_handle("fork");
	if (!son)
		son_pipeline(cmd_line, cmd_meta, i);
	if (cmd_meta->prev_fd != -1)
		close(cmd_meta->prev_fd);
	if (cmd_line->commands[i + 1])
	{
		close(cmd_meta->pipefd[1]);
		cmd_meta->prev_fd = cmd_meta->pipefd[0];
	}
	return (son);
}

/*
* cmd_meta_initializing function
*	- set the starting values of cmd_meta structure
*/
static void	cmd_meta_initializing(t_cmd_meta *cmd_meta)
{
	cmd_meta->input_fd = -1;
	cmd_meta->output_fd = -1;
	cmd_meta->prev_fd = -1;
	cmd_meta->saved_stdin = dup(STDIN_FILENO);
	if (cmd_meta->saved_stdin < 0)
		error_handle("dup");
	cmd_meta->saved_stdout = dup(STDOUT_FILENO);
	if (cmd_meta->saved_stdout < 0)
		error_handle("dup");
}

/*
* launch_pipeline function :
*	- launches all commands in a pipeline and stores their process IDs
*	- initializes command metadata before execution
*	- stops if the command line is marked as interrupted
*/
pid_t	*launch_pipeline(t_cmd_line *cmd_line, t_cmd_meta *cmd_meta)
{
	int		i;
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * cmd_line->n_commands);
	if (!pids)
		error_handle("malloc");
	cmd_line->pids = pids;
	cmd_meta_initializing(cmd_meta);
	i = 0;
	while (cmd_line->commands[i] && !cmd_line->is_interrupted)
	{
		pids[i] = pipeline_iteration(cmd_line, cmd_meta, i);
		i++;
	}
	return (pids);
}
