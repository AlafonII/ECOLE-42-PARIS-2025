/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 16:06:08 by alex              #+#    #+#             */
/*   Updated: 2025/04/14 15:02:56 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* process_single_quote function :
*	- handles single-quoted sections in a token
*	- wraps the content between special markers '\1' and '\2'
*	- updates the input string pointer and token index
*/
static void	process_single_quote(const char **str_ptr, char *token, int *i)
{
	const char	*str;

	str = *str_ptr;
	token[(*i)++] = '\1';
	str++;
	while (*str && *str != '\'')
	{
		token[(*i)++] = *str;
		str++;
	}
	if (*str == '\'')
		str++;
	token[(*i)++] = '\2';
	*str_ptr = str;
}

/*
* process_double_quote function :
*	- handles double-quoted sections in a token
*	- copies characters inside quotes into the token
*	- updates the input string pointer and token index
*/
static void	process_double_quote(const char **str_ptr, char *token, int *i)
{
	const char	*str;

	token[(*i)++] = '\3';
	str = *str_ptr;
	str++;
	while (*str && *str != '\"')
	{
		token[(*i)++] = *str;
		str++;
	}
	if (*str == '\"')
		str++;
	token[(*i)++] = '\4';
	*str_ptr = str;
}

/*
* handle_redirection function :
*	- processes redirection operators ('<' or '>')
*   - including double forms ('<<' or '>>')
*	- appends them to the token and updates the input string pointer
*	- returns the updated token index
*/

static int	handle_redirection(const char **str_ptr, char *token, int i)
{
	const char	*str;

	str = *str_ptr;
	token[i++] = *str++;
	if ((*str == '<' || *str == '>') && token[0] == *str)
		token[i++] = *str++;
	*str_ptr = str;
	return (i);
}

/*
* extract_token_body function :
*	- parses characters from the input string to build a token
*	- handles redirections, single and double quotes appropriately
*	- updates the input pointer and token index as it processes
*/
void	extract_token_body(const char **str_ptr, char *token, int *i)
{
	const char	*str;

	str = *str_ptr;
	while (*str && !ft_isspace(*str))
	{
		if (*str == '<' || *str == '>')
		{
			if (*i == 0)
				*i = handle_redirection(&str, token, *i);
			break ;
		}
		else if (*str == '\'' || *str == '\"')
		{
			if (*str == '\'')
				process_single_quote(&str, token, i);
			else
				process_double_quote(&str, token, i);
		}
		else
			token[(*i)++] = *str++;
	}
	*str_ptr = str;
}
