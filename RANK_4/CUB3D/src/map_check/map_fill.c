/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_fill.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:59:04 by alex              #+#    #+#             */
/*   Updated: 2025/05/29 22:49:16 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: map_fill.c
 * ---------------------------------------------------------------
 * Replace spaces in each map row with '0' to fill whitespace.
 * ---------------------------------------------------------------
 */

/*
 * Function: replace_whitespace
 * Role: Replace all space chars in map rows with '0'.
 *
 * Parameters:
 *   map - Null-terminated array of map row strings.
 *
 * Returns:
 *   Pointer to modified map array.
 */
char	**replace_whitespace(char **map)
{
	int	rows;
	int	i;
	int	j;

	rows = 0;
	while (map[rows])
		rows++;
	i = 0;
	while (i < rows)
	{
		if (i != 0 && i != rows - 1)
		{
			j = 0;
			while (map[i][j])
			{
				if (map[i][j] == ' ')
					map[i][j] = '0';
				j++;
			}
		}
		i++;
	}
	return (map);
}
