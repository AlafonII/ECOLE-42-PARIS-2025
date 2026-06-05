/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 10:51:36 by alex              #+#    #+#             */
/*   Updated: 2025/06/03 12:33:36 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: image.c
 * ---------------------------------------------------------------
 * Display images to window
 * ---------------------------------------------------------------
 */

/*
 * Function: ray_drawing
 * Role: draw rays
 *
 * Parameters:
 *	 cfg - struct s_data
 *	 image - struct s_image
 *	 racast - struct s_raycast
 */
static void	ray_drawing(t_image *image, t_raycast *raycast,
				t_data_texture *texture, double ratio)
{
	int		*data;
	int		tw;
	int		th;
	char	*dst;

	data = texture->data;
	tw = texture->width;
	th = texture->height;
	while (raycast->wall_start < raycast->wall_end)
	{
		dst = image->addr + (raycast->wall_start * image->line_len
				+ raycast->pixel_col * image->bpp / 8);
		*(unsigned int *)dst = data[((int)texture->row % th)
			* tw + texture->col];
		raycast->wall_start++;
		texture->row += ratio;
	}
}

/*
 * Function: wall_drawing
 * Role: draw walls
 *
 * Parameters:
 *	 cfg - struct s_data
 *	 image - struct s_image
 *	 racast - struct s_raycast
 */
static void	wall_drawing(t_data *cfg, t_image *image, t_raycast *raycast)
{
	double			ratio;
	t_data_texture	texture;

	texture = cfg->textures.datatex[raycast->side_impact];
	ratio = (double)texture.height / raycast->real_size;
	texture.col = find_texture_col(cfg, raycast);
	if (raycast->real_size > raycast->size)
		texture.row = ratio * (raycast->real_size - raycast->size) / 2;
	else
		texture.row = 0;
	ray_drawing(image, raycast, &texture, ratio);
}

/*
 * Function: floor_drawing
 * Role: trace the floor portion of ray
 *
 * Parameters:
 *	 cfg - struct s_data
 *	 image - struct s_image
 *	 racast - struct s_raycast
 */
void	floor_drawing(t_data *cfg, t_image *img, t_raycast *raycast)
{
	int		y;
	int		color;
	char	*dst;

	color = rgb_to_int(cfg->textures.floor_color);
	y = raycast->wall_end;
	while (++y < WIN_HEIGHT)
	{
		dst = img->addr + (y * img->line_len + raycast->pixel_col
				* img->bpp / 8);
		*(unsigned int *)dst = color;
	}
}

/*
 * Function: ceil_drawing
 * Role: trace the ceil portion of ray
 *
 * Parameters:
 *	cfg - structure s_data
 *	img - structure s_image
 *	raycast - structure s_raycasting
 */
void	ceil_drawing(t_data *cfg, t_image *img, t_raycast *raycast)
{
	int		y;
	int		color;
	char	*dst;

	color = rgb_to_int(cfg->textures.ceiling_color);
	y = 0;
	while (y < raycast->wall_start)
	{
		dst = img->addr + (y * img->line_len + raycast->pixel_col
				* img->bpp / 8);
		*(unsigned int *)dst = color;
		y++;
	}
}

/*
 * Function: raytracing
 * Role: trace the wall potion of ray
 *
 * Parameters:
 *	cfg - structure s_data
 *	img - structure s_image
 *	raycast - structure s_raycasting
 */
void	raytracing(t_data *cfg, t_image *img, t_raycast *raycast)
{
	raycast->wall_start = (WIN_HEIGHT - raycast->size) / 2;
	raycast->wall_end = raycast->wall_start + raycast->size;
	if (raycast->size)
	{
		ceil_drawing(cfg, img, raycast);
		wall_drawing(cfg, img, raycast);
		floor_drawing(cfg, img, raycast);
	}
}
