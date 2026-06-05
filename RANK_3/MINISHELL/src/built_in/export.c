/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:50:09 by alex              #+#    #+#             */
/*   Updated: 2025/04/19 21:06:17 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* copy_env function :
*	- creates a shallow copy of the environment array
*	- allocates memory for the array but not for the strings
*	- returns the copied array or NULL on allocation failure
*/
static char	**copy_env(char **env)
{
	int		count;
	char	**sorted;
	int		i;

	count = 0;
	while (env[count])
		count++;
	sorted = malloc(sizeof(char *) * (count + 1));
	if (!sorted)
		return (NULL);
	i = 0;
	while (i < count)
	{
		sorted[i] = env[i];
		i++;
	}
	sorted[i] = NULL;
	return (sorted);
}

/*
* sort_env function :
*	- sorts the given environment array in alphabetical order
*	- uses insertion sort algorithm for in-place sorting
*/
static void	sort_env(char **sorted)
{
	int		i;
	int		j;
	char	*temp;

	i = 1;
	while (sorted[i])
	{
		temp = sorted[i];
		j = i;
		while (j > 0 && ft_strcmp(sorted[j - 1], temp) > 0)
		{
			sorted[j] = sorted[j - 1];
			j--;
		}
		sorted[j] = temp;
		i++;
	}
}

/*
* print_env_line function :
*	- formats and prints an environment variable in export style
*	- skips printing the LAST_EXIT variable
*	- adds quotes around values if an '=' is present
*/
static void	print_env_line(char *line)
{
	char	*equal_ptr;

	if (ft_strncmp(line, "LAST_EXIT=", 10) != 0)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		equal_ptr = ft_strchr(line, '=');
		if (equal_ptr)
		{
			write(STDOUT_FILENO, line, equal_ptr - line);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(equal_ptr + 1, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		else
			ft_putstr_fd(line, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
}

/*
* print_sorted_env function :
*	- prints the environment variables in alphabetical order
*	- uses helper functions to copy and sort the env array
*	- prints each variable to stdout and frees the sorted copy
*/
void	print_sorted_env(char **env)
{
	int		i;
	char	**sorted;

	sorted = copy_env(env);
	if (!sorted)
		return ;
	sort_env(sorted);
	i = 0;
	while (sorted[i])
	{
		print_env_line(sorted[i]);
		i++;
	}
	free(sorted);
}

/*
* ft_export function :
*	- reproduce the behavior of the export command
*	- adds or updates environment variables if arguments are given
*	- prints the sorted environment if no arguments are provided
*	- returns an error for invalid identifiers or update failure
*/
int	ft_export(char **args, char ***envp)
{
	int	i;

	if (!args[1])
		return (print_sorted_env(*envp), 0);
	i = 1;
	while (args[i])
	{
		if (!is_valid_id(args[i]))
		{
			ft_putendl_fd("export: not a valid identifier", STDERR_FILENO);
			return (1);
		}
		if (ft_strchr(args[i], '='))
		{
			if (update_env(envp, args[i]) != 0)
				return (1);
		}
		i++;
	}
	return (0);
}
