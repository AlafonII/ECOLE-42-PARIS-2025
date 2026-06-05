/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:14:26 by alex              #+#    #+#             */
/*   Updated: 2025/02/20 15:25:39 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

static int	give_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

static void	my_mlx_pixel_put(t_img *img, int x, int y)
{
	char	*dst;

	if (x < 0 || y < 0 || x >= WIN_WIDTH || y >= WIN_HEIGHT)
		return ;
	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = 0xFFFFFF;
}

static void	init_line(t_line *line, t_point2d p1, t_point2d p2)
{
	line->dx = give_abs(p2.x - p1.x);
	line->dy = give_abs(p2.y - p1.y);
	if (p1.x < p2.x)
		line->sx = 1;
	else
		line->sx = -1;
	if (p1.y < p2.y)
		line->sy = 1;
	else
		line->sy = -1;
	line->err = line->dx - line->dy;
}

static void	draw_line_internal(t_mlx *mlx, t_point2d p1, t_point2d p2)
{
	t_line	line;
	int		e2;

	init_line(&line, p1, p2);
	while (p1.x != p2.x || p1.y != p2.y)
	{
		my_mlx_pixel_put(mlx->img, p1.x, p1.y);
		e2 = 2 * line.err;
		if (e2 > -line.dy)
		{
			line.err -= line.dy;
			p1.x += line.sx;
		}
		if (e2 < line.dx)
		{
			line.err += line.dx;
			p1.y += line.sy;
		}
	}
}

void	draw_map(t_mlx *mlx, t_point2d **map, int rows, int cols)
{
	int	x;
	int	y;

	y = 0;
	while (y < rows)
	{
		x = 0;
		while (x < cols)
		{
			if (x < cols - 1)
				draw_line_internal(mlx, map[y][x], map[y][x + 1]);
			if (y < rows - 1)
				draw_line_internal(mlx, map[y][x], map[y + 1][x]);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img->img, 0, 0);
}
