/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:12:32 by alafon            #+#    #+#             */
/*   Updated: 2025/05/29 22:49:16 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: parse_texture.c
 * ---------------------------------------------------------------
 * Parse texture header lines and store texture paths.
 * ---------------------------------------------------------------
 */

/*
 * Function: parse_texture_line
 * Role: Parse a texture header line and set the destination path.
 *
 * Parameters:
 *   line - Raw header line string.
 *   id   - Expected texture identifier ("NO","SO","WE","EA").
 *   dst  - Address of destination pointer for texture path.
 */
static int	parse_texture_line(char *line, const char *id, char **dst)
{
	char	*p;

	if (ft_strncmp(line, id, 2) != 0 || !ft_isspace(line[2]))
		return (FAILURE);
	p = line + 2;
	while (ft_isspace((char)*p))
		p++;
	if (*p == '\0')
		return (FAILURE);
	*dst = ft_strdup(p);
	if (!*dst)
		return (FAILURE);
	return (SUCCESS);
}

/*
 * Function: check_and_parse
 * Role: Prevent duplicate texture ID then parse texture line.
 *
 * Parameters:
 *   trim  - Trimmed header line string.
 *   key   - Expected texture ID string.
 *   field - Address of texture path pointer to populate.
 */
static int	check_and_parse(const char *trim, char *key, char **field)
{
	if (*field)
		return (FAILURE);
	if (parse_texture_line((char *)trim, key, field) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

/*
 * Function: parse_texture_headers
 * Role: Identify texture header and parse its path.
 *
 * Parameters:
 *   trim     - Trimmed header line starting with ID.
 *   textures - Pointer to t_textures struct to populate.
 */
int	parse_texture_headers(const char *trim, t_textures *textures)
{
	if (!ft_strncmp(trim, "NO", 2))
	{
		if (check_and_parse(trim, "NO", &textures->no) == FAILURE)
			return (FAILURE);
	}
	else if (!ft_strncmp(trim, "SO", 2))
	{
		if (check_and_parse(trim, "SO", &textures->so) == FAILURE)
			return (FAILURE);
	}
	else if (!ft_strncmp(trim, "WE", 2))
	{
		if (check_and_parse(trim, "WE", &textures->we) == FAILURE)
			return (FAILURE);
	}
	else if (!ft_strncmp(trim, "EA", 2))
	{
		if (check_and_parse(trim, "EA", &textures->ea) == FAILURE)
			return (FAILURE);
	}
	else
		return (FAILURE);
	return (SUCCESS);
}
