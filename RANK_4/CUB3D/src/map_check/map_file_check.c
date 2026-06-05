/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_file_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 19:04:06 by alex              #+#    #+#             */
/*   Updated: 2025/05/29 22:49:16 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: map_file_check.c
 * ---------------------------------------------------------------
 * Checks .cub extension and ensures filename isn't a directory.
 * Verifies that the file can be opened for reading.
 * ---------------------------------------------------------------
 */

/*
 * Function: check_cub_format
 * Role: Verify that filename ends with ".cub" extension.
 *
 * Parameters:
 *   filename - Null-terminated string with the file name.
 *
 * Returns:
 *   SUCCESS if filename has valid ".cub" extension; otherwise FAILURE.
 */
static int	check_cub_format(const char *filename)
{
	int	len;

	if (!filename)
		return (FAILURE);
	len = 0;
	while (filename[len])
		len++;
	if (len < 5)
		return (FAILURE);
	if (filename[len - 4] != '.' || filename[len - 3] != 'c'
		|| filename[len - 2] != 'u' || filename[len - 1] != 'b')
		return (FAILURE);
	return (SUCCESS);
}

/*
 * Function: is_dir
 * Role: Determine if the given filename refers to a directory.
 *
 * Parameters:
 *   filename - Null-terminated string with file or directory name.
 *
 * Returns:
 *   true if filename is a directory; false otherwise.
 */
static bool	is_dir(const char *filename)
{
	int		fd;
	bool	ret;

	ret = false;
	fd = open(filename, O_DIRECTORY);
	if (fd >= 0)
	{
		close(fd);
		ret = true;
	}
	return (ret);
}

/*
 * Function: map_file_check
 * Role: Validate that filename has .cub extension, is not a directory,
 * and can be opened for reading.
 *
 * Parameters:
 *   filename - Null-terminated string for file to check.
 *
 * Returns:
 *   SUCCESS if file passes all checks; otherwise FAILURE.
 */
int	map_file_check(const char *filename)
{
	int	fd;

	if (check_cub_format(filename))
		return (FAILURE);
	if (is_dir((char *)filename))
		return (FAILURE);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (FAILURE);
	close(fd);
	return (SUCCESS);
}
