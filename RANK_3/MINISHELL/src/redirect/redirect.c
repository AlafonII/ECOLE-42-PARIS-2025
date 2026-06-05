/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:01:49 by jguillem          #+#    #+#             */
/*   Updated: 2025/04/20 12:12:33 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_next_token(t_cmd_line *cmd_line, int i, int j)
{
	if (!cmd_line->commands[i][j + 1])
	{
		ft_putstr_fd("-minishell"BAD_EOL, 2);
		free_command_line(cmd_line);
		free_envp(cmd_line->envp);
		exit(2);
	}
	else if (is_redirection(cmd_line->commands[i][j + 1]))
	{
		ft_putstr_fd("-minishell"REDIR_ERROR, 2);
		ft_putstr_fd(cmd_line->commands[i][j + 1], 2);
		ft_putstr_fd("'\n", 2);
		free_command_line(cmd_line);
		free_envp(cmd_line->envp);
		exit(2);
	}
}

/*
* in_redir function :
*	- manage cmd < file command type
*	- open file with O_RDONLY flag
*	- shift command when "< file cmd" syntax
*	- replace stdin by file descriptor
*/
void	in_redir(t_cmd_line *cmd_line, t_cmd_meta *cmd_meta, int i, int j)
{
	char	*filename;

	check_next_token(cmd_line, i, j);
	filename = ft_strdup(cmd_line->commands[i][j + 1]);
	process_tok(&filename, cmd_line->envp);
	cmd_meta->input_fd = open(filename, O_RDONLY);
	if (cmd_meta->input_fd < 0)
		error_handle("open");
	if (dup2(cmd_meta->input_fd, STDIN_FILENO) < 0)
		error_handle("dup2");
	close(cmd_meta->input_fd);
	free(filename);
}

/*
* out_redir function :
*	- manage cmd > file command type
*	- create file with O_TRUNC flag
*	- replace stdout by file descriptor
*/
void	out_redir(t_cmd_line *cmd_line, t_cmd_meta *cmd_meta, int i, int j)
{
	char	*filename;

	check_next_token(cmd_line, i, j);
	filename = ft_strdup(cmd_line->commands[i][j + 1]);
	process_tok(&filename, cmd_line->envp);
	cmd_meta->output_fd = open(filename,
			O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (cmd_meta->output_fd < 0)
		error_handle("open");
	if (dup2(cmd_meta->output_fd, STDOUT_FILENO) < 0)
		error_handle("dup2");
	close(cmd_meta->output_fd);
	free(filename);
}

/*
* out_append_redir function :
*	- manage cmd >> file command type
*	- create file with O_APPEND flag
*	- replace stdout by file descriptor
*/
void	out_append_redir(t_cmd_line *cmd_line,
	t_cmd_meta *cmd_meta, int i, int j)
{
	char	*filename;

	check_next_token(cmd_line, i, j);
	filename = ft_strdup(cmd_line->commands[i][j + 1]);
	process_tok(&filename, cmd_line->envp);
	cmd_meta->output_fd = open(filename,
			O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (cmd_meta->output_fd < 0)
		error_handle("open");
	if (dup2(cmd_meta->output_fd, STDOUT_FILENO) < 0)
		error_handle("dup2");
	close(cmd_meta->output_fd);
	free(filename);
}
