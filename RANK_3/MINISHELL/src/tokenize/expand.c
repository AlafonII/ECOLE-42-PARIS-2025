/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 16:03:30 by alex              #+#    #+#             */
/*   Updated: 2025/04/17 20:27:50 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* handle_dollar_question function :
*	- handles expansion of the special variable '$?'
*	- retrieves the LAST_EXIT value from the environment or defaults to "0"
*	- appends the value to the result string and returns it
*/
static char	*handle_dollar_question(int *i, char *result, char **envp)
{
	char	*exit_val;
	char	*tmp;

	exit_val = get_env_value_str(envp, "LAST_EXIT");
	if (!exit_val || !exit_val[0])
	{
		free(exit_val);
		exit_val = ft_strdup("0");
	}
	(*i)++;
	tmp = ft_strjoin(result, exit_val);
	free(result);
	free(exit_val);
	return (tmp);
}

/*
* handle_dollar_literal function :
*	- handles a literal dollar sign with no following variable
*	- appends '$' to the result string and returns it
*/
static char	*handle_dollar_literal(char *result)
{
	char	*tmp;

	tmp = ft_strjoin(result, "$");
	free(result);
	return (tmp);
}

/*
* handle_dollar_normal function :
*	- handles standard variable expansion like $VAR
*	- extracts the variable name and retrieves its value from the environment
*	- appends the value to the result string and returns it
*/
static char	*handle_dollar(const char *token, int *i, char *result, char **envp)
{
	int		start;
	char	*varname;
	char	*varvalue;
	char	*tmp;

	start = *i;
	while (token[*i] && (ft_isalnum(token[*i]) || token[*i] == '_'))
		(*i)++;
	varname = ft_substr(token, start, *i - start);
	varvalue = get_env_value_str(envp, varname);
	tmp = ft_strjoin(result, varvalue);
	free(result);
	free(varname);
	free(varvalue);
	return (tmp);
}

/*
* expand_variables_in_token function :
*	- expands environment variables within a token string
*	- handles special cases like '$?', literal '$', and normal variables
*	- builds and returns the expanded result string
*/
char	*expand_variables_in_token(char *token, char **envp)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (token[i])
	{
		if (token[i] == '$')
		{
			i++;
			if (token[i] == '?')
				result = handle_dollar_question(&i, result, envp);
			else if (!token[i] || !(ft_isalnum(token[i]) || token[i] == '_'))
				result = handle_dollar_literal(result);
			else
				result = handle_dollar(token, &i, result, envp);
			continue ;
		}
		result = append_char(result, token[i]);
		i++;
	}
	return (result);
}

/*
* expand_tokens function :
*	- iterates over all command tokens and processes each one
*	- expands environment variables and removes special markers
*	- modifies the commands array in place
*/
void	expand_tokens(char ***commands, char **envp)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (commands[i])
	{
		j = 0;
		k = 0;
		while (commands[i][j])
		{
			process_tok(&commands[i][j], envp);
			if (ft_strlen(commands[i][j]) > 0)
				commands[i][k++] = commands[i][j];
			else
				free(commands[i][j]);
			j++;
		}
		commands[i][k] = NULL;
		i++;
	}
}
