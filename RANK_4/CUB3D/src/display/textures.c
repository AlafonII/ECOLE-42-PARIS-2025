/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 19:13:54 by alex              #+#    #+#             */
/*   Updated: 2025/06/03 19:13:56 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: textures.c
 * ---------------------------------------------------------------
 * Convert RGB to int, load textures, and compute texture column.
 * ---------------------------------------------------------------
 */

/*
 * Function: rgb_to_int
 * Role: Convert RGB array to a packed integer color value.
 * 
 * Parameters:
 *   rgb - Array of three integers [R,G,B] (0-255 each).
 * 
 * Returns:
 *   Packed integer in format (R<<16)|(G<<8)|B.
 */
int	rgb_to_int(int rgb[3])
{
	return (rgb[0] * 65536 + rgb[1] * 256 + rgb[2]);
}

/*
 * Function: load_single_texture
 * Role: Load XPM image into a data_texture slot in cfg.
 * 
 * Parameters:
 *   graph - Pointer to t_graphic with MLX context.
 *   cfg   - Pointer to t_data containing texture storage.
 *   path  - File path to XPM texture.
 *   idx   - Index in cfg->textures.datatex array.
 * 
 * Returns:
 *   SUCCESS if loaded; otherwise FAILURE.
 */
static int	load_single_texture(t_graphic *graph,
			t_data *cfg, char *path, int idx)
{
	t_data_texture	tex;

	tex.image.img = mlx_xpm_file_to_image(graph->mlx, path,
			&tex.width, &tex.height);
	if (!tex.image.img)
		return (FAILURE);
	tex.data = (int *)mlx_get_data_addr(tex.image.img,
			&tex.image.bpp, &tex.image.line_len,
			&tex.image.endian);
	cfg->textures.datatex[idx] = tex;
	return (SUCCESS);
}

/*
 * Function: load_textures
 * Role: Load all four wall textures and handle load failures.
 * 
 * Parameters:
 *   graph - Pointer to t_graphic with MLX context.
 *   cfg   - Pointer to t_data containing texture paths and storage.
 *
 */
void	load_textures(t_graphic *graph, t_data *cfg)
{
	char	*sides[4];
	int		i;

	sides[0] = cfg->textures.no;
	sides[1] = cfg->textures.we;
	sides[2] = cfg->textures.so;
	sides[3] = cfg->textures.ea;
	i = 0;
	while (i < 4)
	{
		if (load_single_texture(graph, cfg, sides[i], i))
		{
			while (i-- > 0)
				mlx_destroy_image(graph->mlx,
					cfg->textures.datatex[i].image.img);
			cleanup_mlx(graph);
			free_config(cfg);
			error_exit(TXTR_ERROR);
		}
		i++;
	}
}

/*
 * Function: find_texture_col
 * Role: Compute horizontal texture column for a given raycast.
 * 
 * Parameters:
 *   cfg     - Pointer to t_data with player and textures.
 *   raycast - Pointer to t_raycast with angle, dist, side impact.
 * 
 * Returns:
 *   Column index into the texture image for rendering.
 */
int	find_texture_col(t_data *cfg, t_raycast *raycast)
{
	double	wall_hit;
	double	wall_hit_y;
	double	wall_hit_x;
	double	ratio;

	wall_hit_x = cfg->player.game_pos_x + cos(raycast->angle) * raycast->dist;
	wall_hit_y = cfg->player.game_pos_y + sin(raycast->angle) * raycast->dist;
	if (raycast->side_impact % 2 == 0)
		wall_hit = wall_hit_x;
	else
		wall_hit = wall_hit_y;
	ratio = fmod(wall_hit, CELL_SIZE) / CELL_SIZE;
	if (ratio < 0)
		ratio += 1.0;
	return ((int)(ratio * cfg->textures.datatex[raycast->side_impact].width));
}
