/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2025/05/17 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: parse_texture_utils.c
 * ---------------------------------------------------------------
 * Utilities to extract directory name and validate texture paths.
 * ---------------------------------------------------------------
 */

/*
 * Function: get_dirname
 * Role: Extract directory part of a file path.
 * 
 * Parameters:
 *   path - Null-terminated file path string.
 * 
 * Returns:
 *   Allocated string with directory name; exits on memory error.
 */
static char	*get_dirname(const char *path)
{
	int		i;
	char	*dir;

	i = ft_strlen(path) - 1;
	while (i >= 0 && path[i] != '/')
		i--;
	if (i < 0)
		dir = ft_strdup(".");
	else
		dir = ft_substr(path, 0, i);
	if (!dir)
		error_exit(MEMORY_ERROR);
	return (dir);
}

/*
 * Function: check_texture_directory
 * Role: Verify that a path exists and is a directory.
 * 
 * Parameters:
 *   dir_path - Path to directory to check.
 * 
 * Returns:
 *   SUCCESS if dir_path is a directory; exits on error.
 */
int	check_texture_directory(const char *dir_path)
{
	struct stat	st;

	if (stat(dir_path, &st) < 0 || !S_ISDIR(st.st_mode))
		return (FAILURE);
	return (SUCCESS);
}

/*
 * Function: check_xpm_extension
 * Role: Ensure file path ends with ".xpm" extension.
 * 
 * Parameters:
 *   file_path - Null-terminated file path string.
 * 
 * Returns:
 *   SUCCESS if extension is valid; exits on error.
 */
int	check_xpm_extension(const char *file_path)
{
	size_t	len;

	len = ft_strlen(file_path);
	if (len < 4 || ft_strncmp(file_path + len - 4, ".xpm", 4) != 0)
		return (FAILURE);
	return (SUCCESS);
}

/*
 * Function: validate_all_textures
 * Role: Validate texture paths by checking directory and extensions.
 * 
 * Parameters:
 *   textures - Pointer to t_textures struct with file paths.
 * 
 * Returns:
 *   SUCCESS if all textures valid; exits on error.
 */
int	validate_all_textures(const t_textures *textures)
{
	const char	*paths[4];
	char		*dir;
	int			i;

	paths[0] = textures->no;
	paths[1] = textures->so;
	paths[2] = textures->we;
	paths[3] = textures->ea;
	i = 0;
	while (i < 4)
	{
		dir = get_dirname(paths[i]);
		if (check_texture_directory(dir) == FAILURE)
			return (free(dir), FAILURE);
		free(dir);
		if (check_xpm_extension(paths[i]) == FAILURE)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}
