/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 19:14:57 by alex              #+#    #+#             */
/*   Updated: 2025/04/19 12:47:51 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* update_last_exit function :
*	- stores the last command's exit status in the environment
*	- converts the status to a string and sets LAST_EXIT variable
*/
void	update_last_exit(char ***envp, int status)
{
	char	*tmp;
	char	*num;

	num = ft_itoa(status);
	tmp = ft_strjoin("LAST_EXIT=", num);
	free(num);
	update_env(envp, tmp);
	free(tmp);
	reorder_last_exit(envp);
}

/*
* get_env_value function :
*	- retrieves the integer value of an environment variable by key
*	- returns 0 if the key is not found or the value is invalid
*/
static int	get_env_value(const char **envp, const char *key)
{
	int		i;
	int		key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, key_len) && envp[i][key_len] == '=')
			return (ft_atoi(envp[i] + key_len + 1));
		i++;
	}
	return (0);
}

/*
* is_valid_number function :
*	- check if a string is a valid numeric value
*	- allows optional leading '+' or '-' signs
*	- returns 1 if the string is numeric, 0 otherwise
*/
static int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
* ft_exit function :
*	- reproduce the behavior of the exit command
*	- handles optional numeric argument for exit status
*	- prints errors for too many arguments or non-numeric values
*	- frees resources and exits the program with the given status
*/
int	ft_exit(t_cmd_line *cmd_line)
{
	int	status;

	status = 0;
	if (!cmd_line->commands[0][1])
		status = get_env_value((const char **)cmd_line->envp, "LAST_EXIT");
	else if (cmd_line->commands[0][2])
		return (ft_putendl_fd("exit: too many arguments", STDERR_FILENO), 1);
	else
	{
		if (!is_valid_number(cmd_line->commands[0][1]))
		{
			ft_putendl_fd("exit: numeric argument required", STDERR_FILENO);
			status = 2;
		}
		else
			status = ft_atoi(cmd_line->commands[0][1]) % 256;
	}
	ft_putendl_fd("exit", STDOUT_FILENO);
	free_command_line(cmd_line);
	free_envp(cmd_line->envp);
	exit(status);
	return (0);
}

void	reorder_last_exit(char ***envp)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], "LAST_EXIT=", 10))
			break ;
		i++;
	}
	if (!(*envp)[i])
		return ;
	temp = (*envp)[i];
	j = i;
	while (j > 0)
	{
		(*envp)[j] = (*envp)[j - 1];
		j--;
	}
	(*envp)[0] = temp;
}
