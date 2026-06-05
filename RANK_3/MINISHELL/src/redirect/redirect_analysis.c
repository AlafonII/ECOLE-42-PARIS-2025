/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_analysis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 21:38:09 by jguillem          #+#    #+#             */
/*   Updated: 2025/04/20 10:55:04 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* is_redirection function :
*	- checks if a string represents a redirection operator
*	- returns 1 for '>', '>>', '<', or '<<'; 0 otherwise
*/
int	is_redirection(char *str)
{
	if (!ft_strcmp(str, ">") || !ft_strcmp(str, ">>")
		|| !ft_strcmp(str, "<") || !ft_strcmp(str, "<<"))
		return (1);
	return (0);
}

/*
 * clean_command function:
 *   - removes redirection tokens and their targets in-place
 *   - frees removed tokens and compacts the command array
 */
void	clean_command(t_cmd_line *cmd_line, int i)
{
	char	**cmd;
	int		read;
	int		write;

	cmd = cmd_line->commands[i];
	read = 0;
	write = 0;
	while (cmd[read])
	{
		if (is_redirection(cmd[read]))
		{
			free(cmd[read++]);
			if (cmd[read])
				free(cmd[read++]);
		}
		else
			cmd[write++] = cmd[read++];
	}
	cmd[write] = NULL;
}

/*
* analyze_redir function :
*	- identifies and handles redirection operators in a command
*	- processes the token following the redirection to expand variables
*	- delegates to the appropriate redirection handler (>, >>, <, <<)
*/
static void	analyze_redir(t_cmd_line *cmd_line,
	t_cmd_meta *cmd_meta,
	int cmd_idx, int tok_idx)
{
	char	*token;

	token = cmd_line->commands[cmd_idx][tok_idx];
	if (!ft_strcmp(token, ">"))
		out_redir(cmd_line, cmd_meta, cmd_idx, tok_idx);
	else if (!ft_strcmp(token, ">>"))
		out_append_redir(cmd_line, cmd_meta, cmd_idx, tok_idx);
	else if (!ft_strcmp(token, "<"))
		in_redir(cmd_line, cmd_meta, cmd_idx, tok_idx);
	else if (!ft_strcmp(token, "<<"))
		heredoc(cmd_line, cmd_meta, cmd_idx, tok_idx);
}

/*
* redirect_analysis function :
*	- scans a command for redirection tokens and processes them
*	- applies the appropriate redirection logic via analyze_redir
*	- cleans the command by removing redirection tokens afterward
*/
void	redirect_analysis(t_cmd_line *cmd_line,
	t_cmd_meta *cmd_meta, int i)
{
	int		j;

	j = 0;
	while (cmd_line->commands[i][j])
	{
		analyze_redir(cmd_line, cmd_meta, i, j);
		j++;
	}
	clean_command(cmd_line, i);
}
