/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:11:57 by alex              #+#    #+#             */
/*   Updated: 2025/05/29 23:15:54 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: parse_map.c
 * ---------------------------------------------------------------
 * Read, process, and store map rows into t_mapinfo; handle errors.
 * ---------------------------------------------------------------
 */

/*
 * Function: add_line
 * Role: Append a trimmed line to the array, reallocating as needed.
 * 
 * Parameters:
 *   lines - Existing array of map row strings.
 *   count - Number of rows currently in lines.
 *   trim  - Trimmed line to append.
 * 
 * Returns:
 *   New array with the appended line; exits on memory error.
 */
static char	**add_line(char **lines, int count, char *trim)
{
	char	**new;
	int		i;

	new = malloc(sizeof(char *) * (count + 2));
	if (new == NULL)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new[i] = lines[i];
		i++;
	}
	new[count] = ft_strdup(trim);
	if (new[count] == NULL)
	{
		free(new);
		return (NULL);
	}
	new[count + 1] = NULL;
	free(lines);
	return (new);
}

/*
 * Function: handle_map_row
 * Role: Validate a trimmed line and append it to info->file.
 * 
 * Parameters:
 *   info - Map info struct tracking file, counts, and flags.
 *   trim - Trimmed line string to process.
 * 
 * Returns:
 *   SUCCESS if line appended; otherwise FAILURE.
 */
static int	handle_map_row(t_mapinfo *info, char *trim)
{
	if (info->blank_after || !is_map_row(trim))
		return (FAILURE);
	info->file = add_line(info->file, info->line_count, trim);
	if (info->file == NULL)
		return (FAILURE);
	info->line_count++;
	info->map_started = 1;
	return (SUCCESS);
}

/*
 * Function: cleanup_failure
 * Role: Free raw line and allocated file on error.
 * 
 * Parameters:
 *   info - Map info struct containing file pointer.
 *   raw  - Raw line string to free.
 * 
 * Returns:
 *   FAILURE always.
 */
static int	cleanup_failure(t_mapinfo *info, char *raw)
{
	free(raw);
	if (info->file)
		free_map(info->file);
	return (FAILURE);
}

/*
 * Function: process_map_line
 * Role: Trim raw input, handle map row or set blank flag.
 * 
 * Parameters:
 *   info - Map info struct tracking parse state.
 *   raw  - Raw line string from file.
 * 
 * Returns:
 *   SUCCESS if processed; FAILURE on invalid map row.
 */
static int	process_map_line(t_mapinfo *info, char *raw)
{
	char	*trim;

	trim = trim_spaces(raw);
	if (*trim)
	{
		if (handle_map_row(info, trim) == FAILURE)
			return (FAILURE);
	}
	else if (info->map_started)
		info->blank_after = 1;
	return (SUCCESS);
}

/*
 * Function: parse_map_lines
 * Role: Read lines, process map, set info->height, handle errors.
 * 
 * Parameters:
 *   info - Map info struct with fd and storage fields.
 * 
 * Returns:
 *   SUCCESS if at least one row parsed; FAILURE on error or empty map.
 */
int	parse_map_lines(t_mapinfo *info)
{
	char	*raw;

	info->file = NULL;
	info->line_count = 0;
	info->map_started = 0;
	info->blank_after = 0;
	raw = get_next_line(info->fd);
	while (raw)
	{
		if (process_map_line(info, raw) == FAILURE)
			return (cleanup_failure(info, raw));
		free(raw);
		raw = get_next_line(info->fd);
	}
	info->height = info->line_count;
	if (info->line_count == 0)
	{
		if (info->file)
			free_map(info->file);
		return (FAILURE);
	}
	return (SUCCESS);
}
