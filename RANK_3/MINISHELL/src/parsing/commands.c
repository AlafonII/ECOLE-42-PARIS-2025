/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 12:40:04 by jguillem          #+#    #+#             */
/*   Updated: 2025/04/17 19:49:46 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* alloc_commands function : alloc memory for char ***commands
*/
static char	***alloc_commands(char *txt)
{
	int		n;
	int		i;
	char	***cmds;

	n = 1;
	while (*txt)
	{
		if (*txt == '|')
			n++;
		txt++;
	}
	cmds = malloc((n + 1) * sizeof(char **));
	if (!cmds)
		return (NULL);
	i = 0;
	while (i < n + 1)
		cmds[i++] = NULL;
	return (cmds);
}

/*
* find_command_end function :
*	- finds the end of a command segment in a string
*	- skips pipe characters inside quotes
*	- returns a pointer to the pipe or end of the string
*/
static const char	*find_command_end(const char *s)
{
	char		quote;
	const char	*scout;

	quote = '\0';
	scout = s;
	while (*scout)
	{
		if ((*scout == '\'' || *scout == '\"') && quote == '\0')
			quote = *scout;
		else if (*scout == quote)
			quote = '\0';
		else if (*scout == '|' && quote == '\0')
			break ;
		scout++;
	}
	return (scout);
}

/*
* extract_command function :
*	- extracts a single command segment from a string, handling quotes
*	- splits the segment into tokens and updates the input pointer
*	- uses find_command_end to avoid splitting on pipes inside quotes
*/
static char	**extract_command(char **txt_ptr)
{
	char		*segment;
	char		**tokens;
	const char	*end;

	while (**txt_ptr && ft_isspace(**txt_ptr))
		(*txt_ptr)++;
	end = find_command_end(*txt_ptr);
	segment = ft_substr(*txt_ptr, 0, end - *txt_ptr);
	if (!segment)
		return (NULL);
	tokens = tokenize_args(segment);
	free(segment);
	if (*end)
		*txt_ptr = (char *)end + 1;
	else
		*txt_ptr = (char *)end;
	return (tokens);
}

/*
* split_commands function :
*	- splits a command line string into an array of command tokens
*	- uses extract_command to separate commands by '|'
*	- returns a NULL-terminated array of command arrays
*/
void	split_commands(t_cmd_line *cmd_line, char *txt)
{
	char	***cmds;
	int		i;

	cmd_line->commands = NULL;
	cmd_line->n_commands = 0;
	cmds = alloc_commands(txt);
	if (!cmds)
		return ;
	i = 0;
	while (*txt)
	{
		cmds[i] = extract_command(&txt);
		if (!cmds[i])
			return ;
		i++;
	}
	cmd_line->commands = cmds;
	cmd_line->n_commands = i;
}
