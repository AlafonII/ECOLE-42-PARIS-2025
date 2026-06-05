/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 19:39:39 by alex              #+#    #+#             */
/*   Updated: 2025/04/13 13:10:12 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* ft_env function :
*	- reproduce the behavior of the env command
*	- prints the current environment variables
*	- returns an error if extra arguments are provided
*/
int	ft_env(char **argv, char **envp)
{
	int	i;

	(void)argv;
	i = 0;
	if (argv && argv[1])
		return (ft_putstr_fd("Too many arguments\n", STDERR_FILENO), 1);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "LAST_EXIT=", 10) != 0)
		{
			ft_putstr_fd(envp[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		i++;
	}
	return (0);
}
