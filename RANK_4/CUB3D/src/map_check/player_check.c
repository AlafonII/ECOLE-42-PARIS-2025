/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 19:15:59 by alex              #+#    #+#             */
/*   Updated: 2025/05/29 22:49:16 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: player_check.c
 * ---------------------------------------------------------------
 * Check presence and locate single player marker with orientation.
 * ---------------------------------------------------------------
 */

/*
 * Function: check_player
 * Role: Count directional markers to ensure exactly one player.
 * 
 * Parameters:
 *   map - Null-terminated array of map row strings.
 * 
 * Returns:
 *   SUCCESS if exactly one marker; otherwise FAILURE.
 */
static int	check_player(char **map)

{
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == 'N'
				|| map[i][j] == 'S'
				|| map[i][j] == 'W'
				|| map[i][j] == 'E')
				count++;
			j++;
		}
		i++;
	}
	if (count != 1)
		return (FAILURE);
	return (SUCCESS);
}

/*
 * Function: find_player_marker
 * Role: Locate player marker and return its coordinates and direction.
 * 
 * Parameters:
 *   map - Null-terminated array of map row strings.
 *   pi  - Pointer to store row index of player.
 *   pj  - Pointer to store column index of player.
 *   dir - Pointer to store direction char of player.
 * 
 * Returns:
 *   SUCCESS if marker found; otherwise FAILURE.
 */
static int	find_player_marker(char **map, int *pi, int *pj, char *dir)

{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == 'N' || map[i][j] == 'S'
			|| map[i][j] == 'E' || map[i][j] == 'W')
			{
				*pi = i;
				*pj = j;
				*dir = map[i][j];
				return (SUCCESS);
			}
			j++;
		}
		i++;
	}
	return (FAILURE);
}

/*
 * Function: set_player_data
 * Role: Set player position and view direction in data struct.
 * 
 * Parameters:
 *   data - Pointer to t_data struct to update.
 *   i    - Row index of player.
 *   j    - Column index of player.
 *   dir  - Direction char ('N','S','E','W').
 */
static void	set_player_data(t_data *data, int i, int j, char dir)
{
	data->player.pos_x = j + 0.5;
	data->player.pos_y = i + 0.5;
	data->player.game_pos_x = data->player.pos_x * CELL_SIZE;
	data->player.game_pos_y = data->player.pos_y * CELL_SIZE;
	if (dir == 'N')
		data->player.player_view = DIR_N;
	else if (dir == 'S')
		data->player.player_view = DIR_S;
	else if (dir == 'E')
		data->player.player_view = DIR_E;
	else if (dir == 'W')
		data->player.player_view = DIR_O;
}

/*
 * Function: set_player_pos
 * Role: Find marker, set player coordinates and view.
 * 
 * Parameters:
 *   map  - Null-terminated array of map row strings.
 *   data - Pointer to t_data struct to update.
 * 
 * Returns:
 *   SUCCESS if position set; otherwise FAILURE.
 */
static int	set_player_pos(char **map, t_data *data)
{
	int		i;
	int		j;
	char	dir;

	if (find_player_marker(map, &i, &j, &dir) == FAILURE)
		return (FAILURE);
	set_player_data(data, i, j, dir);
	return (SUCCESS);
}

/*
 * Function: check_and_set_player
 * Role: Validate single player and record its position.
 * 
 * Parameters:
 *   map  - Null-terminated array of map row strings.
 *   data - Pointer to t_data struct to update.
 * 
 * Returns:
 *   SUCCESS if checks pass; otherwise FAILURE.
 */
int	check_and_set_player(char **map, t_data *data)
{
	if (check_player(map) == FAILURE
		|| set_player_pos(map, data) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
