/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:18:51 by alex              #+#    #+#             */
/*   Updated: 2025/06/12 18:14:26 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: bfs_map.c
 * ---------------------------------------------------------------
 * Find player start, count rows, get max columns, and read map chars.
 * ---------------------------------------------------------------
 */

/*
 * Function: find_player
 * Role: Locate unique player marker and record its indices.
 * 
 * Parameters:
 *   map - NULL-terminated array of map row strings.
 *   si  - Pointer to store found row index.
 *   sj  - Pointer to store found column index.
 * 
 * Returns:
 *   1 if exactly one marker found; otherwise 0.
 */
int	find_player(char **map, int *si, int *sj)
{
	int	i;
	int	j;
	int	count;

	count = 0;
	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == 'N' || map[i][j] == 'S'
			|| map[i][j] == 'E' || map[i][j] == 'W')
			{
				*si = i;
				*sj = j;
				count++;
			}
			j++;
		}
		i++;
	}
	return (count == 1);
}

/*
 * Function: map_rows
 * Role: Count number of rows in a NULL-terminated map array.
 * 
 * Parameters:
 *   map - NULL-terminated array of map row strings.
 * 
 * Returns:
 *   Number of rows.
 */
int	map_rows(char **map)
{
	int	i;

	i = 0;
	while (map[i])
		i++;
	return (i);
}

/*
 * Function: get_max_cols
 * Role: Determine the maximum row length in the map array.
 * 
 * Parameters:
 *   map - NULL-terminated array of map row strings.
 * 
 * Returns:
 *   Maximum string length among all rows.
 */
int	get_max_cols(char **map)
{
	int		rows;
	size_t	max;
	int		i;
	size_t	len;

	rows = map_rows(map);
	max = 0;
	i = 0;
	while (i < rows)
	{
		len = ft_strlen(map[i]);
		if (len > max)
			max = len;
		i++;
	}
	return (max);
}
