/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:25:52 by alafon            #+#    #+#             */
/*   Updated: 2025/06/03 12:33:47 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: free.c
 * ---------------------------------------------------------------
 * Free allocated config, map, graphics, and texture resources.
 * ---------------------------------------------------------------
 */

/*
 * Function: free_config
 * Role: Free textures, map, and config structure.
 *
 * Parameters:
 *   cfg - Pointer to t_data; if NULL, does nothing.
 */
void	free_config(t_data *cfg)
{
	if (!cfg)
		return ;
	free(cfg->textures.no);
	free(cfg->textures.so);
	free(cfg->textures.we);
	free(cfg->textures.ea);
	if (cfg->map)
		free_map(cfg->map);
	free(cfg);
}

/*
 * Function: free_map
 * Role: Free null-terminated array of strings.
 *
 * Parameters:
 *   map - Array of strings to free; if NULL, does nothing.
 */
void	free_map(char **map)
{
	int	i;

	i = 0;
	if (!map)
		return ;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}

/*
 * Function: cleanup_mlx
 * Role: Destroy MLX window, display and free context.
 *
 * Parameters:
 *   graph - Pointer to t_graphic with MLX context.
 */
void	cleanup_mlx(t_graphic *graph)
{
	if (graph->mlx)
	{
		if (graph->win)
			mlx_destroy_window(graph->mlx, graph->win);
		mlx_destroy_display(graph->mlx);
		free(graph->mlx);
		graph->mlx = NULL;
	}
}

/*
 * Function: destroy_textures
 * Role: Destroy all loaded texture images in MLX.
 *
 * Parameters:
 *   param - t_game pointer cast to void*.
 */
void	destroy_textures(void *param)
{
	t_game		*game;
	t_graphic	*graph;
	t_data		*data;
	int			i;

	game = (t_game *)param;
	graph = game->graph;
	data = game->data;
	i = 0;
	while (i < 4)
	{
		if (data->textures.datatex[i].image.img)
			mlx_destroy_image(graph->mlx,
				data->textures.datatex[i].image.img);
		i++;
	}
}

/*
 * Function: close_window
 * Role: Clean up all resources and exit successfully.
 *
 * Parameters:
 *   param - t_game pointer cast to void*.
 *
 * Returns:
 *   Does not return; exits with SUCCESS.
 */
int	close_window(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	destroy_textures(param);
	cleanup_mlx(game->graph);
	free_config(game->data);
	exit(SUCCESS);
}
