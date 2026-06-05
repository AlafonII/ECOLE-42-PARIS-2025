/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:22:40 by alex              #+#    #+#             */
/*   Updated: 2025/04/02 21:28:42 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* is_valid_id function :
*	- checks if a string is a valid environment variable identifier
*	- must start with a letter or '_', followed by alphanums or underscores
*	- stops checking at the '=' character if present
*/
int	is_valid_id(const char *str)
{
	int	i;

	if (!str || !((str[0] >= 'A' && str[0] <= 'Z')
			|| (str[0] >= 'a' && str[0] <= 'z') || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!((str[i] >= 'A' && str[i] <= 'Z')
				|| (str[i] >= 'a' && str[i] <= 'z')
				|| (str[i] >= '0' && str[i] <= '9')
				|| (str[i] == '_')))
			return (0);
		i++;
	}
	return (1);
}

/*
* remove_env_var function :
*	- removes a variable from the environment array if it matches the name
*	- shifts remaining variables to fill the gap
*	- returns 0 whether the variable is found or not
*/
static int	remove_env_var(char ***envp, char *var)
{
	int		i;
	int		var_len;

	var_len = 0;
	while (var[var_len] && var[var_len] != '=')
		var_len++;
	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], var, var_len)
			&& (((*envp)[i][var_len] == '=') || ((*envp)[i][var_len] == '\0')))
		{
			free((*envp)[i]);
			while ((*envp)[i])
			{
				(*envp)[i] = (*envp)[i + 1];
				i++;
			}
			return (0);
		}
		i++;
	}
	return (0);
}

/*
* ft_unset function :
*	- reproduce the behavior of the unset command
*	- removes environment variables matching the given identifiers
*	- returns an error if any identifier is invalid
*/
int	ft_unset(char **args, char ***envp)
{
	int	i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		if (!is_valid_id(args[i]))
		{
			ft_putendl_fd("unset: not a valid identifier", STDERR_FILENO);
			return (1);
		}
		remove_env_var(envp, args[i]);
		i++;
	}
	return (0);
}
