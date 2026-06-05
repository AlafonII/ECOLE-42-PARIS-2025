/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 20:36:37 by alex              #+#    #+#             */
/*   Updated: 2025/02/20 15:25:36 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	free_map(t_point3d **map, int rows)
{
	int	i;

	i = 0;
	while (i < rows)
	{
		free(map[i]);
		i++;
	}
	free(map);
}

void	free_proj_map(t_point2d **proj_map, int rows)
{
	int	i;

	i = 0;
	while (i < rows)
	{
		free(proj_map[i]);
		i++;
	}
	free(proj_map);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	cleanup_mlx(t_fdf *fdf)
{
	if (fdf->mlx)
	{
		if (fdf->mlx->win)
			mlx_destroy_window(fdf->mlx->mlx, fdf->mlx->win);
		if (fdf->mlx->img)
		{
			if (fdf->mlx->img->img)
				mlx_destroy_image(fdf->mlx->mlx, fdf->mlx->img->img);
			free(fdf->mlx->img);
		}
		if (fdf->mlx->mlx)
		{
			mlx_destroy_display(fdf->mlx->mlx);
			free(fdf->mlx->mlx);
		}
		free(fdf->mlx);
		fdf->mlx = NULL;
	}
}

void	cleanup_all(t_fdf *fdf)
{
	if (fdf->proj_map)
	{
		free_proj_map(fdf->proj_map, fdf->rows);
		fdf->proj_map = NULL;
	}
	if (fdf->map)
	{
		free_map(fdf->map, fdf->rows);
		fdf->map = NULL;
	}
	cleanup_mlx(fdf);
}
