/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:26:27 by alex              #+#    #+#             */
/*   Updated: 2025/04/17 20:27:50 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* get_env_value_str function :
*	- retrieves the value of an environment variable by key
*	- returns a duplicated string of the value if found
*	- returns an empty string if the key is not present
*/
char	*get_env_value_str(char **envp, char *key)
{
	int		key_len;
	int		i;
	char	*value;

	key_len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, key_len) && envp[i][key_len] == '=')
		{
			value = ft_strdup(envp[i] + key_len + 1);
			return (value);
		}
		i++;
	}
	return (ft_strdup(""));
}

/*
* remove_markers function :
*	- removes special marker characters '\1' and '\2' from the token
*	- returns a newly allocated string without the markers
*/
static char	*remove_markers(char *token)
{
	char	*new;
	int		i;
	int		j;

	new = malloc(ft_strlen(token) + 1);
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	while (token[i])
	{
		if (token[i] != '\1' && token[i] != '\2'
			&& token[i] != '\3' && token[i] != '\4')
			new[j++] = token[i];
		i++;
	}
	new[j] = '\0';
	return (new);
}

/*
* handle_literal function :
*	- appends characters to result until marker '\2' is found
*	- used for handling literal sections of a token
*	- returns the updated result string
*/
char	*handle_literal(const char *token, int *i, char *result)
{
	char	*tmp;
	char	temp[2];

	while (token[*i] && token[*i] != '\2')
	{
		temp[0] = token[*i];
		temp[1] = '\0';
		tmp = ft_strjoin(result, temp);
		free(result);
		result = tmp;
		(*i)++;
	}
	if (token[*i] == '\2')
		(*i)++;
	return (result);
}

/*
* append_char function :
*	- appends a single character to the end of a string
*	- returns a newly allocated string with the character added
*/
char	*append_char(char *result, char c)
{
	char	temp[2];
	char	*tmp;

	temp[0] = c;
	temp[1] = '\0';
	tmp = ft_strjoin(result, temp);
	free(result);
	return (tmp);
}

/*
* process_tok function :
*	- processes a token by removing markers and expanding variables if needed
*	- skips expansion for fully literal tokens marked with '\1' and '\2'
*	- updates the token in place with the processed result
*/
void	process_tok(char **token, char **envp)
{
	char	*tmp;

	if ((*token)[0] == '\1' &&
		(*token)[ft_strlen(*token) - 1] == '\2')
	{
		tmp = remove_markers(*token);
		free(*token);
		*token = tmp;
	}
	else
	{
		if (ft_strchr(*token, '$'))
		{
			tmp = expand_variables_in_token(*token, envp);
			free(*token);
			*token = tmp;
		}
		tmp = remove_markers(*token);
		free(*token);
		*token = tmp;
	}
}
