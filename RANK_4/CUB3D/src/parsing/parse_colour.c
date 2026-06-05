/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colour.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:13:55 by alafon            #+#    #+#             */
/*   Updated: 2025/06/12 20:15:13 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: parse_colour.c
 * ---------------------------------------------------------------
 * Parse 'F' and 'C' color headers, extract and validate RGB values.
 * Errors exit on invalid format or out-of-range components.
 * ---------------------------------------------------------------
 */

/*
 * Function: parse_color_component
 * Role: Parse integer color component at index and validate range 0-255.
 *
 * Parameters:
 *   line - String with color data.
 *   i    - Pointer to current parse index, updated past digits.
 *   col  - Pointer to integer to store component.
 *
 * Returns:
 *   SUCCESS on valid component; FAILURE if not digit.
 */
static int	parse_color_component(char *line, int *i, int *col)
{
	long	v;

	if (!ft_isdigit((char)line[*i]))
		return (FAILURE);
	v = ft_atoi(&line[*i]);
	if (v < 0 || v > 255)
		return (FAILURE);
	*col = (int)v;
	while (ft_isdigit((char)line[*i]))
		(*i)++;
	return (SUCCESS);
}

/*
 * Function: parse_color_identifier
 * Role: Check line ID char and skip whitespace after it.
 *
 * Parameters:
 *   line - String starting with ID and whitespace.
 *   id   - Expected identifier char ('F' or 'C').
 *   i    - Pointer to index set past ID and spaces.
 *
 * Returns:
 *   SUCCESS if identifier correct; otherwise FAILURE.
 */
static int	parse_color_identifier(char *line, char id, int *i)
{
	if (line[0] != id || !ft_isspace((char)line[1]))
		return (FAILURE);
	*i = 1;
	while (ft_isspace((char)line[*i]))
		(*i)++;
	return (SUCCESS);
}

/*
 * Function: parse_color_values
 * Role: Parse 3 color components, separated by commas, skip spaces.
 *
 * Parameters:
 *   line - String with components.
 *   i    - Starting index after identifier.
 *   col  - Array to store 3 parsed color ints.
 *
 * Returns:
 *   SUCCESS if all components valid; otherwise FAILURE.
 */
static int	parse_color_values(char *line, int i, int col[3])
{
	int	k;

	k = 0;
	while (k < 3)
	{
		if (parse_color_component(line, &i, &col[k]) == FAILURE)
			return (FAILURE);
		k++;
		if (k < 3)
		{
			if (line[i] != ',')
				return (FAILURE);
			i++;
			while (ft_isspace((char)line[i]))
				i++;
		}
	}
	while (line[i])
	{
		if (!ft_isspace((char)line[i]))
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

/*
 * Function: parse_color_line
 * Role: Parse a color definition line for given ID into col array.
 *
 * Parameters:
 *   line - Trimmed string starting with ID.
 *   id   - Identifier char ('F'/'C').
 *   col  - Array to store RGB values.
 */
static int	parse_color_line(char *line, char id, int col[3])
{
	int	i;

	i = 0;
	if (parse_color_identifier(line, id, &i) == FAILURE)
		return (FAILURE);
	if (parse_color_values(line, i, col) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

/*
 * Function: parse_color_headers
 * Role: Identify header type and fill appropriate color array.
 *
 * Parameters:
 *   trim     - Trimmed header line starting with 'F' or 'C'.
 *   textures - t_textures struct with color arrays.
 */
int	parse_color_headers(const char *trim, t_textures *textures)
{
	if (trim[0] == 'F')
	{
		if (parse_color_line((char *)trim, 'F', textures->floor_color)
			== FAILURE)
			return (FAILURE);
	}
	else if (trim[0] == 'C')
	{
		if (parse_color_line((char *)trim, 'C', textures->ceiling_color)
			== FAILURE)
			return (FAILURE);
	}
	else
		return (FAILURE);
	return (SUCCESS);
}
