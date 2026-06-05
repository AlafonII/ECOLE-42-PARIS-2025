/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:26:35 by alex              #+#    #+#             */
/*   Updated: 2025/04/14 15:01:55 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* skip_whitespace function :
*	- skips whitespace characters in a string
*	- returns a pointer to the first non-whitespace character
*/
static const char	*skip_whitespace(const char *str)
{
	while (*str && ft_isspace(*str))
		str++;
	return (str);
}

/*
* count_tokens function :
*	- counts the number of tokens in a given input line
*	- uses extract_token to iterate and free each token
*	- returns the total number of tokens found
*/
int	count_tokens(const char *line)
{
	int			count;
	char		*tok;
	const char	*ptr;

	count = 0;
	ptr = line;
	tok = extract_token(&ptr);
	while (tok != NULL)
	{
		count++;
		free(tok);
		tok = extract_token(&ptr);
	}
	return (count);
}

/*
* extract_token function :
*	- extracts a token from the input line, skipping leading whitespace
*	- delegates the character parsing to extract_token_body
*	- returns the extracted token and updates the input pointer
*/
char	*extract_token(const char **line_ptr)
{
	const char	*str;
	char		*token;
	int			i;

	str = skip_whitespace(*line_ptr);
	if (!*str)
		return (NULL);
	token = malloc(ft_strlen(str) + 1);
	if (!token)
		return (NULL);
	i = 0;
	extract_token_body(&str, token, &i);
	token[i] = '\0';
	*line_ptr = str;
	return (token);
}

/*
* tokenize_args function :
*	- splits the input line into tokens using extract_token
*	- allocates an array of strings to store the tokens
*	- returns the array of tokens or NULL on allocation failure
*/
char	**tokenize_args(char *line)
{
	int			nb_tokens;
	int			i;
	char		**tokens;
	const char	*ptr;
	char		*tok;

	nb_tokens = count_tokens(line);
	tokens = malloc(sizeof(char *) * (nb_tokens + 1));
	if (!tokens)
		return (NULL);
	ptr = line;
	i = 0;
	tok = extract_token(&ptr);
	while (tok != NULL)
	{
		tokens[i] = tok;
		i++;
		tok = extract_token(&ptr);
	}
	tokens[i] = NULL;
	return (tokens);
}
