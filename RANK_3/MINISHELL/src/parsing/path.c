/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:03:30 by jguillem          #+#    #+#             */
/*   Updated: 2025/04/16 12:48:15 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* extract_paths function : fill the paths variable with paths
*/
static void	extract_paths(char *txt, char **paths)
{
	int	i;
	int	j;
	int	n_paths;

	i = 0;
	while (txt[i] != '=')
		i++;
	if (txt[i + 1] == '\0')
	{
		paths[0] = ft_strdup("");
		return ;
	}
	j = ++i;
	n_paths = 0;
	while (txt[i])
	{
		while (txt[j] && txt[j] != ':')
			j++;
		paths[n_paths] = ft_substr(txt, i, j - i);
		n_paths++;
		if (txt[j])
			i = ++j;
		else
			i = j;
	}
}

/*
* alloc_paths function : alloc memory for the number of paths in $PATH variable
*/
static char	**alloc_paths(char *txt)
{
	int		j;
	int		n_paths;
	char	**paths;

	j = 0;
	n_paths = 0;
	while (txt[j])
	{
		if (txt[j] == ':')
			n_paths++;
		j++;
	}
	paths = malloc(sizeof(char *) * (n_paths + 2));
	if (!paths)
		return (NULL);
	paths[n_paths + 1] = NULL;
	return (paths);
}

/*
* load_paths function : extract the differents paths from environnement
*/
char	**load_paths(char **envp)
{
	int		i;
	char	**paths;

	i = -1;
	paths = NULL;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			paths = alloc_paths(envp[i]);
			if (!paths)
				return (NULL);
			extract_paths(envp[i], paths);
			break ;
		}
	}
	if (!paths)
	{
		paths = malloc(sizeof(char *));
		if (paths)
			paths[0] = NULL;
	}
	return (paths);
}

/*
* find_path function : find the path of a linux command
*/
char	*find_path(char *pgme, char **paths)
{
	char	*prefix;
	char	*path;

	if (!paths)
		return (NULL);
	while (pgme && *paths)
	{
		prefix = ft_strjoin(*paths, "/");
		path = ft_strjoin(prefix, pgme);
		free(prefix);
		if (access(path, F_OK) == 0)
			return (path);
		else
			free(path);
		paths++;
	}
	return (NULL);
}
