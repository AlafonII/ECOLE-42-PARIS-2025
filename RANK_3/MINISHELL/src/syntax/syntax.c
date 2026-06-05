/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:46:52 by jguillem          #+#    #+#             */
/*   Updated: 2025/04/20 15:14:35 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	pipe_error_exit(t_cmd_line *cmd_line, char ***envp_ptr)
{
	ft_putstr_fd("-minishell"PIPE_ERROR, 2);
	update_last_exit(&(cmd_line->envp), 2);
	*envp_ptr = cmd_line->envp;
	free_command_line(cmd_line);
	return (1);
}

static int	pipe_following_redir(char *txt)
{
	if (*(txt + 1) && *txt == *(txt + 1))
		txt++;
	txt++;
	while (*txt && ft_isspace(*txt))
		txt++;
	if (*txt == '|')
		return (1);
	return (0);
}

static int	check_quote(char **txt)
{
	char	quote_type;

	if (**txt == '"' || **txt == 39)
	{
		quote_type = **txt;
		(*txt)++;
		while (**txt && **txt != quote_type)
			(*txt)++;
		if (**txt == quote_type)
			(*txt)++;
		else
			return (1);
	}
	return (0);
}

int	cmd_line_syntax_analysis(char *txt, t_cmd_line *cmd_line, char ***envp_ptr)
{
	while (*txt)
	{
		while (*txt && ft_isspace(*txt))
			txt++;
		if (*txt == '|')
			return (pipe_error_exit(cmd_line, envp_ptr));
		while (*txt && *txt != '|')
		{
			if (check_quote(&txt))
				return (0);
			if (*txt == '<' || *txt == '>')
				if (pipe_following_redir(txt))
					return (pipe_error_exit(cmd_line, envp_ptr));
			if (*txt)
				txt++;
		}
		if (*(txt) == '|' && txt++)
		{
			while (*txt && ft_isspace(*txt))
				txt++;
			if (*txt == '\0' || *txt == '|')
				return (pipe_error_exit(cmd_line, envp_ptr));
		}
	}
	return (0);
}
