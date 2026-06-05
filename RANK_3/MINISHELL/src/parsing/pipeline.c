/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:46:22 by alex              #+#    #+#             */
/*   Updated: 2025/04/19 19:30:35 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* collect_pipeline_status function :
*	- waits for all child processes in the pipeline to finish
*	- captures the exit status of each and marks interruption if needed
*	- returns the exit status of the last command
*/
static int	collect_pipeline_status(t_cmd_line *cmd_line,
	int n_commands, pid_t *pids)
{
	int	i;
	int	last_status;
	int	child_status;
	int	sig;

	i = 0;
	while (i < n_commands)
	{
		waitpid(pids[i], &child_status, 0);
		if (WIFSIGNALED(child_status))
		{
			sig = WTERMSIG(child_status);
			if (sig == SIGINT)
				printf("\n");
			if (WTERMSIG(child_status) == SIGQUIT)
				printf("Quit (core dumped)\n");
			last_status = 128 + sig;
		}
		if (WIFEXITED(child_status))
			last_status = WEXITSTATUS(child_status);
		if (child_status)
			cmd_line->is_interrupted = 1;
		i++;
	}
	return (last_status);
}

/*
* pipeline function :
*	- launches and manages the execution of a command pipeline
*	- waits for all child processes and collects their exit statuses
*	- updates the LAST_EXIT environment variable with the final status
*/
void	pipeline(t_cmd_line *cmd_line)
{
	t_cmd_meta	cmd_meta;
	pid_t		*pids;
	int			last_status;

	pids = launch_pipeline(cmd_line, &cmd_meta);
	last_status
		= collect_pipeline_status(cmd_line, cmd_line->n_commands, pids);
	if (dup2(cmd_meta.saved_stdin, STDIN_FILENO) < 0)
		error_handle("dup2");
	if (dup2(cmd_meta.saved_stdout, STDOUT_FILENO) < 0)
		error_handle("dup2");
	close(cmd_meta.saved_stdin);
	close(cmd_meta.saved_stdout);
	update_last_exit(&(cmd_line->envp), last_status);
	free(pids);
}
