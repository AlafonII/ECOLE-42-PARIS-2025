/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iso_projection_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 17:15:24 by alex              #+#    #+#             */
/*   Updated: 2025/02/20 15:26:04 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	update_xy_bounds(t_projection *p, double iso_x, double iso_y)
{
	if (iso_x < p->min_x)
		p->min_x = iso_x;
	if (iso_x > p->max_x)
		p->max_x = iso_x;
	if (iso_y < p->min_y)
		p->min_y = iso_y;
	if (iso_y > p->max_y)
		p->max_y = iso_y;
}

void	parse_map_bounds(t_projection *p)
{
	int		x;
	int		y;
	double	iso_x;
	double	iso_y;

	init_bounds(p);
	y = 0;
	while (y < p->rows)
	{
		x = 0;
		while (x < p->cols)
		{
			iso_x = (p->map[y][x].x - p->map[y][x].y) * cos(ISO_ANGLE);
			iso_y = (p->map[y][x].x + p->map[y][x].y) * sin(ISO_ANGLE)
				- p->map[y][x].z;
			update_xy_bounds(p, iso_x, iso_y);
			x++;
		}
		y++;
	}
}

void	compute_scale(t_projection *p)
{
	double	width;
	double	height;
	double	scale_x;
	double	scale_y;

	width = p->max_x - p->min_x;
	height = p->max_y - p->min_y;
	scale_x = WIN_WIDTH / width;
	scale_y = WIN_HEIGHT / height;
	if (scale_x < scale_y)
		p->scale = 0.9 * scale_x;
	else
		p->scale = 0.9 * scale_y;
}

void	compute_offsets(t_projection *p)
{
	double	width;
	double	height;
	int		w_scaled;
	int		h_scaled;

	width = p->max_x - p->min_x;
	height = p->max_y - p->min_y;
	w_scaled = (int)(width * p->scale);
	h_scaled = (int)(height * p->scale);
	p->x_off = (WIN_WIDTH - w_scaled) / 2 - (int)(p->min_x * p->scale);
	p->y_off = (WIN_HEIGHT - h_scaled) / 2 - (int)(p->min_y * p->scale);
}

void	apply_projection(t_projection *p)
{
	int	x;
	int	y;

	y = 0;
	while (y < p->rows)
	{
		x = 0;
		while (x < p->cols)
		{
			p->proj[y][x] = project_iso_custom(
					p->map[y][x],
					p->scale,
					p->x_off,
					p->y_off);
			x++;
		}
		y++;
	}
}
