/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iso_projection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:08:11 by alex              #+#    #+#             */
/*   Updated: 2025/02/20 15:26:10 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

t_point2d	project_iso_custom(t_point3d point,
		double scale, int x_offset, int y_offset)
{
	t_point2d	proj;
	double		iso_x;
	double		iso_y;

	iso_x = (point.x - point.y) * cos(ISO_ANGLE);
	iso_y = (point.x + point.y) * sin(ISO_ANGLE) - point.z;
	proj.x = (int)(iso_x * scale + x_offset);
	proj.y = (int)(iso_y * scale + y_offset);
	return (proj);
}

void	init_bounds(t_projection *p)
{
	double	iso_x;
	double	iso_y;

	if (p->rows > 0 && p->cols > 0)
	{
		iso_x = (p->map[0][0].x - p->map[0][0].y) * cos(ISO_ANGLE);
		iso_y = (p->map[0][0].x + p->map[0][0].y) * sin(ISO_ANGLE)
			- p->map[0][0].z;
		p->min_x = iso_x;
		p->max_x = iso_x;
		p->min_y = iso_y;
		p->max_y = iso_y;
	}
	else
		return ;
}

void	project_map( t_point3d **map, t_point2d **proj, int rows, int cols)
{
	t_projection	p;

	p.map = map;
	p.proj = proj;
	p.rows = rows;
	p.cols = cols;
	parse_map_bounds(&p);
	compute_scale(&p);
	compute_offsets(&p);
	apply_projection(&p);
}
