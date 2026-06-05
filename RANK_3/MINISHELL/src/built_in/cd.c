/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 14:51:44 by alex              #+#    #+#             */
/*   Updated: 2025/04/03 14:57:33 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* get_target_directory function :
*	- determines the target directory for the cd command
*	- uses HOME from env if no argument is given
*	- handles errors for unset HOME or too many arguments
*/
static int	get_target_directory(char **argv, char ***envp, char **target)
{
	int	i;

	if (!argv[1])
	{
		i = 0;
		while ((*envp)[i])
		{
			if (!ft_strncmp((*envp)[i], "HOME=", 5))
			{
				*target = (*envp)[i] + 5;
				return (0);
			}
			i++;
		}
		ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
		return (1);
	}
	else if (argv[1] && argv[2])
	{
		ft_putendl_fd("cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	*target = argv[1];
	return (0);
}

/*
* get_old_pwd function :
*	- retrieves the current working directory using getcwd
*	- returns the directory path or NULL on failure
*	- prints an error message if getcwd fails
*/
static char	*get_old_pwd(void)
{
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		perror("getcwd");
		return (NULL);
	}
	return (old_pwd);
}

/*
* update_pwd_env function :
*	- updates the OLDPWD and PWD environment variables
*	- builds the assignment strings using ft_strjoin
*	- returns 1 on memory allocation failure, 0 on success
*/
static int	update_pwd_env(char *old_pwd, char *pwd, char ***envp)
{
	char	*temp;

	temp = ft_strjoin("OLDPWD=", old_pwd);
	if (!temp)
		return (1);
	update_env(envp, temp);
	free(temp);
	temp = ft_strjoin("PWD=", pwd);
	if (!temp)
		return (1);
	update_env(envp, temp);
	free(temp);
	return (0);
}

/*
* ft_cd function :
*	- reproduce the behavior of the cd command
*	- changes the current directory to the target path
*	- updates PWD and OLDPWD environment variables on success
*	- handles and prints errors for invalid targets or failures
*/
int	ft_cd(char **argv, char ***envp)
{
	char	*target;
	char	*old_pwd;
	char	*pwd;

	if (get_target_directory(argv, envp, &target))
		return (1);
	old_pwd = get_old_pwd();
	if (!old_pwd)
		return (1);
	if (chdir(target) != 0)
	{
		perror("cd");
		free(old_pwd);
		return (1);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("getcwd");
		free(old_pwd);
		return (1);
	}
	update_pwd_env(old_pwd, pwd, envp);
	return (free(old_pwd), free(pwd), 0);
}
