/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:58:56 by jguillem          #+#    #+#             */
/*   Updated: 2025/04/21 21:40:37 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* line_process function :
*	- write each line of the heredoc to the write-end pipe
*	- stop when the keywork delimiter is recognized
*	- stop if CTRL+D and display an alert message
*/
static void	lines_process(char *line, char *eof, int write_fd)
{
	int		i;

	i = 1;
	while (1)
	{
		if (!line)
		{
			printf("minishell: warning: "HDOC_WRNG_BEG" %d "\
				HDOC_WRNG_MED"%s')\n", i, eof);
			break ;
		}
		if (!ft_strcmp(line, eof))
		{
			free(eof);
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
		line = readline("> ");
		i++;
	}
}

/*
* heredoc function (son level of the fork)
*	- reinitialize the originals stdin and stdout
*	- launch the line_process function for manage the heredoc
*	- put back stdout for continue the pipeline
*/
static void	son_heredoc(t_cmd_meta *cmd_meta, char *eof,
			int pipefd[2], int is_last_cmd)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	close(pipefd[0]);
	if (dup2(cmd_meta->saved_stdout, STDOUT_FILENO) < 0)
		error_handle("dup2");
	if (dup2(cmd_meta->saved_stdin, STDIN_FILENO) < 0)
		error_handle("dup2");
	line = readline("> ");
	lines_process(line, eof, pipefd[1]);
	if (!is_last_cmd)
		if (dup2(cmd_meta->pipefd[1], STDOUT_FILENO) < 0)
			error_handle("dup2");
	close(pipefd[1]);
	exit(EXIT_SUCCESS);
}

/*
* father_heredoc function : handle return status of the son level
*/
static void	father_heredoc(t_cmd_line *cmd_line, t_cmd_meta *cmd_meta,
			int pipefd[2], int status)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		if (dup2(cmd_meta->saved_stdout, STDOUT_FILENO) < 0)
			error_handle("dup2");
		printf("\n");
		close(pipefd[0]);
		free_command_line(cmd_line);
		exit(130);
	}
	else
	{
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) < 0)
			error_handle("dup2");
		cmd_meta->input_fd = dup(pipefd[0]);
		if (cmd_meta->input_fd < 0)
			error_handle("dup");
		close(pipefd[0]);
	}
}

/*
* check_next_redirection_token function :
*	- check if token of redirection is the last
*	- check if the following token is also a redirection one
*/
static void	check_next_redirection_token(char *next_token)
{
	if (!next_token)
	{
		ft_putstr_fd("-minishell"BAD_EOL, 2);
		exit(2);
	}
	if (is_redirection(next_token))
	{
		ft_putstr_fd("-minishell"REDIR_ERROR, 2);
		exit(2);
	}
}

/*
* heredoc function
*	- fork a son process for write in a pipe
*	- put back stdout for continue the pipeline
*	- after fork, read in the read end of pipeline at parent level
*/
void	heredoc(t_cmd_line *cmd_line, t_cmd_meta *cmd_meta, int i, int j)
{
	int		pipefd[2];
	pid_t	son;
	int		status;
	int		is_last_cmd;
	char	*eof;

	ignore_signals();
	is_last_cmd = 1;
	if (cmd_line->commands[i + 1])
		is_last_cmd = 0;
	check_next_redirection_token(cmd_line->commands[i][j + 1]);
	eof = ft_strdup(cmd_line->commands[i][j + 1]);
	process_tok(&eof, cmd_line->envp);
	if (pipe(pipefd) == -1)
		error_handle("pipe");
	son = fork();
	if (son < 0)
		error_handle("fork");
	if (!son)
		son_heredoc(cmd_meta, eof,
			pipefd, is_last_cmd);
	waitpid(son, &status, 0);
	free(eof);
	father_heredoc(cmd_line, cmd_meta, pipefd, status);
}
