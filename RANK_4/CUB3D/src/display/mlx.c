/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:11:26 by jguillem          #+#    #+#             */
/*   Updated: 2025/05/29 22:49:16 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: mlx.c
 * ---------------------------------------------------------------
 * Manage the mlx window
 * Opening and closing the mlx window
 * ---------------------------------------------------------------
 */

/*
 * Function: graphic_initializing
 * Role: Load the mlx structure
 *
 * Parameters:
 *   graph - struct s_graphic
 *
 * Returns:
 *   Nothing
 */
void	graphic_initializing(t_graphic *graph)
{
	graph->mlx = mlx_init();
	if (!graph->mlx)
		error_exit(ERR_MLX_INIT);
	graph->win = mlx_new_window(graph->mlx, WIN_WIDTH, WIN_HEIGHT, "CUBE3D");
	if (!graph->win)
	{
		free(graph->mlx);
		error_exit(ERR_WIN_CREATE);
	}
}

/*
 * Function: key_action
 * Role: react to keyboard input
 *
 * Parameters:
 *   keycode - int
 *   graph - void *
 *
 * Returns:
 *   0 if success
 */
int	key_action(int keycode, void *graph)
{
	if (keycode == XK_Escape)
		close_window(graph);
	return (SUCCESS);
}

/*
 * Function: image_initializing 
 * Role: alloc memory for image
 *
 * Parameters:
 *   graph - struct s_graphic
 *	 image - struct s_image
 *	 width - int
 *	 height - int
 */
void	image_initializing(t_graphic *graph, t_image *image,
			int width, int height)
{
	image->img = mlx_new_image(graph->mlx, width, height);
	if (!image->img)
	{
		mlx_destroy_window(graph->mlx, graph->win);
		mlx_destroy_display(graph->mlx);
		free(graph->mlx);
		error_exit(ERR_IMG_CREATE);
	}
	image->addr = mlx_get_data_addr(image->img, &image->bpp,
			&image->line_len, &image->endian);
}
