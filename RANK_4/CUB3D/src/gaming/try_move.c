/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   try_move.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:54:18 by alafon            #+#    #+#             */
/*   Updated: 2025/05/29 22:49:16 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: try_move.c
 * ---------------------------------------------------------------
 * Detect collisions with walls and move player if path clear.
 * ---------------------------------------------------------------
 */

/*
 * Function: check_side_collision
 * Role: Check for side collisions using collision buffer.
 * 
 * Parameters:
 *   d  - Pointer to game data with map info.
 *   nx - Proposed new x-coordinate.
 *   ny - Proposed new y-coordinate.
 * 
 * Returns:
 *   1 if side collision detected; otherwise 0.
 */
static int	check_side_collision(t_data *d, double nx, double ny)
{
	int		cx;
	int		cy;
	double	ox;
	double	oy;

	cx = (int)(nx / CELL_SIZE);
	cy = (int)(ny / CELL_SIZE);
	ox = nx - cx * CELL_SIZE;
	oy = ny - cy * CELL_SIZE;
	if (ox < COLLISION_BUFFER && d->map[cy][cx - 1] == '1')
		return (FAILURE);
	if (ox > CELL_SIZE - COLLISION_BUFFER && d->map[cy][cx + 1] == '1')
		return (FAILURE);
	if (oy < COLLISION_BUFFER && d->map[cy - 1][cx] == '1')
		return (FAILURE);
	if (oy > CELL_SIZE - COLLISION_BUFFER && d->map[cy + 1][cx] == '1')
		return (FAILURE);
	return (SUCCESS);
}

/*
 * Function: check_corner_collision
 * Role: Check for corner collisions using collision buffer.
 * 
 * Parameters:
 *   d  - Pointer to game data with map info.
 *   nx - Proposed new x-coordinate.
 *   ny - Proposed new y-coordinate.
 * 
 * Returns:
 *   1 if corner collision detected; otherwise 0.
 */
static int	check_corner_collision(t_data *d, double nx, double ny)
{
	int		cx;
	int		cy;
	double	ox;
	double	oy;

	cx = (int)(nx / CELL_SIZE);
	cy = (int)(ny / CELL_SIZE);
	ox = nx - cx * CELL_SIZE;
	oy = ny - cy * CELL_SIZE;
	if (ox < COLLISION_BUFFER && oy < COLLISION_BUFFER
		&& d->map[cy - 1][cx - 1] == '1')
		return (FAILURE);
	if (ox < COLLISION_BUFFER && oy > CELL_SIZE - COLLISION_BUFFER
		&& d->map[cy + 1][cx - 1] == '1')
		return (FAILURE);
	if (ox > CELL_SIZE - COLLISION_BUFFER && oy < COLLISION_BUFFER
		&& d->map[cy - 1][cx + 1] == '1')
		return (FAILURE);
	if (ox > CELL_SIZE - COLLISION_BUFFER && oy > CELL_SIZE - COLLISION_BUFFER
		&& d->map[cy + 1][cx + 1] == '1')
		return (FAILURE);
	return (SUCCESS);
}

/*
 * Function: try_move
 * Role: Attempt to move player by dx, dy if no collisions.
 * 
 * Parameters:
 *   d  - Pointer to game data with player and map.
 *   dx - Movement delta on x axis.
 *   dy - Movement delta on y axis.
 */
void	try_move(t_data *d, double dx, double dy)
{
	double	nx;
	double	ny;
	int		cx;
	int		cy;

	nx = d->player.game_pos_x + dx;
	ny = d->player.game_pos_y + dy;
	cx = (int)(nx / CELL_SIZE);
	cy = (int)(ny / CELL_SIZE);
	if (d->map[cy][cx] == '1')
		return ;
	if (check_side_collision(d, nx, ny) || check_corner_collision(d, nx, ny))
		return ;
	d->player.game_pos_x = nx;
	d->player.game_pos_y = ny;
}
