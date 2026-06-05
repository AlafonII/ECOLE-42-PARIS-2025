/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 12:45:02 by alex              #+#    #+#             */
/*   Updated: 2025/04/17 20:44:10 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* add_new_env function :
*	- adds a new environment variable to the env array
*	- reallocates the array and appends the new assignment
*	- returns 0 on success or 1 on memory allocation failure
*/
static int	add_new_env(char ***envp, char *assignment)
{
	int		i;
	char	**new_env;

	i = 0;
	while ((*envp) && (*envp)[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (1);
	i = 0;
	while ((*envp) && (*envp)[i])
	{
		new_env[i] = (*envp)[i];
		i++;
	}
	new_env[i] = ft_strdup(assignment);
	new_env[i + 1] = NULL;
	if (*envp)
		free(*envp);
	*envp = new_env;
	return (0);
}

/*
* update_env function :
*	- updates an existing environment variable or adds a new one
*	- compares variable names up to the '=' character
*	- delegates to add_new_env if the variable is not found or env is not init
*/
int	update_env(char ***envp, char *assignment)
{
	int		i;
	int		name_len;

	if (!envp || !(*envp))
		return (add_new_env(envp, assignment));
	name_len = 0;
	while (assignment[name_len] && assignment[name_len] != '=')
		name_len++;
	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], assignment, name_len) &&
			(((*envp)[i][name_len] == '=') ||
			((*envp)[i][name_len] == '\0')))
		{
			free((*envp)[i]);
			(*envp)[i] = ft_strdup(assignment);
			return (0);
		}
		i++;
	}
	return (add_new_env(envp, assignment));
}
