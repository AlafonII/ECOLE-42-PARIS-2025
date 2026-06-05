/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:57:46 by jguillem          #+#    #+#             */
/*   Updated: 2025/05/29 23:16:46 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: raycasting.c
 * ---------------------------------------------------------------
 * Manage the raycasting
 * ---------------------------------------------------------------
 */

/*
 * Function: distance calibration
 * Role:
 *	define a unit of distance (projection_plane)
 *	increase the size for off of center angles
 *
 * Parameters:
 *	 raycast - struct s_raycast
 *   cfg - struct s_data
 */
static void	distance_calibration(t_raycast *raycast, t_data *cfg)
{
	double	projection_plane;
	double	corrected_dist;

	raycast->dist = fmin(raycast->next_x, raycast->next_y);
	projection_plane = (WIN_WIDTH / 2.0) / tan(FOV / 2.0);
	corrected_dist = raycast->dist
		* cos(raycast->angle - cfg->player.player_view);
	if (corrected_dist < 0.0001)
		corrected_dist = 0.0001;
	raycast->size = (CELL_SIZE * projection_plane) / corrected_dist;
	raycast->real_size = raycast->size;
	if (raycast->size > WIN_HEIGHT)
		raycast->size = WIN_HEIGHT;
	if (raycast->size < 1)
		raycast->size = 1;
}

/*
 * Function: find_side_impact
 * Role: find the side impact of the ray for choose the color of the wall
 *
 * Parameters:
 *	 direction - int
 *   angle - double
 */
static int	find_side_impact(int direction, double angle)
{
	angle = fmod(angle, 2 * DIR_O);
	if (angle < 0)
		angle += 2 * DIR_O;
	if (direction == LINE)
	{
		if (angle > DIR_E && angle < DIR_O)
			return (SOUTH);
		else
			return (NORTH);
	}
	else
	{
		if (angle > DIR_S && angle < DIR_N)
			return (WEST);
		else
			return (EAST);
	}
}

/*
 * Function: raycasting
 * Role: calculate distance of next wall
 *
 * Parameters:
 *	 raycast - struct s_raycast
 *   cfg - struct s_data
 */
static void	raycasting(t_raycast *ray, t_data *cfg)
{
	char	**map;

	map = cfg->map;
	while (map[ray->row][ray->col] != '1')
	{
		ray->next_x = 2147483647;
		ray->next_y = 2147483647;
		if (fabs(cos(ray->angle)) > 0.00001)
			ray->next_x = (ray->delta_x + CELL_SIZE
					* abs(ray->col_init - ray->col)) / fabs(cos(ray->angle));
		if (fabs(sin(ray->angle)) > 0.00001)
			ray->next_y = (ray->delta_y + CELL_SIZE
					* abs(ray->row_init - ray->row)) / fabs(sin(ray->angle));
		if (ray->next_x < ray->next_y)
		{
			ray->side_impact = find_side_impact(COLUMN, ray->angle);
			ray->col += ray->incr_x;
		}
		else
		{
			ray->side_impact = find_side_impact(LINE, ray->angle);
			ray->row += ray->incr_y;
		}
	}
	distance_calibration(ray, cfg);
}

/*
 * Function: raycast_setting
 * Role: collect data for raycasting
 *
 * Parameters:
 *	 raycast - struct s_raycast
 *	 player  - struct s_player
 */
static void	raycast_setting(t_raycast *ray, t_player *player)
{
	ray->col = (int)(player->game_pos_x / CELL_SIZE);
	ray->row = (int)(player->game_pos_y / CELL_SIZE);
	ray->col_init = ray->col;
	ray->row_init = ray->row;
	ray->incr_x = 1;
	ray->incr_y = 1;
	ray->delta_x = (ray->col + 1) * CELL_SIZE - player->game_pos_x;
	ray->delta_y = (ray->row + 1) * CELL_SIZE - player->game_pos_y;
	if (cos(ray->angle) < 0)
	{
		ray->incr_x = -1;
		ray->delta_x = player->game_pos_x - ray->col * CELL_SIZE;
	}
	if (sin(ray->angle) < 0)
	{
		ray->incr_y = -1;
		ray->delta_y = player->game_pos_y - ray->row * CELL_SIZE;
	}
}

/*
 * Function: rayswepting
 * Role: raycasting for each column of pixel on the window
 *
 * Parameters:
 *   graph - struct s_graphic
 *   cfg - struct s_data
 */
void	rayswepting(t_graphic *graph, t_data *cfg)
{
	t_player	player;
	t_raycast	raycast;
	t_image		image;
	double		step;

	image_initializing(graph, &image, WIN_WIDTH, WIN_HEIGHT);
	player = cfg->player;
	step = FOV / WIN_WIDTH;
	raycast.angle = player.player_view - FOV / 2;
	raycast.pixel_col = 0;
	while (raycast.angle <= player.player_view + FOV / 2)
	{
		raycast_setting(&raycast, &player);
		raycasting(&raycast, cfg);
		raytracing(cfg, &image, &raycast);
		raycast.angle += step;
		raycast.pixel_col++;
	}
	mlx_put_image_to_window(graph->mlx, graph->win, image.img, 0, 0);
	mlx_destroy_image(graph->mlx, image.img);
}
