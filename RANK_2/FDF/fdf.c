/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:07:26 by alex              #+#    #+#             */
/*   Updated: 2025/02/24 09:40:03 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	setup_fdf(t_fdf *fdf, char *filename)
{
	int	i;

	fdf->rows = 0;
	fdf->cols = 0;
	fdf->proj_map = NULL;
	fdf->map = parse_fdf(filename, &fdf->rows, &fdf->cols);
	if (!fdf->map)
		exit(1);
	fdf->proj_map = malloc(fdf->rows * sizeof(t_point2d *));
	if (!fdf->proj_map)
		exit(1);
	i = 0;
	while (i < fdf->rows)
	{
		fdf->proj_map[i] = malloc(fdf->cols * sizeof(t_point2d));
		if (!fdf->proj_map[i])
			exit(1);
		i++;
	}
	project_map(fdf->map, fdf->proj_map, fdf->rows, fdf->cols);
}

void	start_mlx(t_fdf *fdf)
{
	fdf->mlx = init_mlx(WIN_WIDTH, WIN_HEIGHT);
	if (!fdf->mlx)
	{
		printf("Error: Could not initialize MiniLibX.\n");
		exit(1);
	}
	draw_map(fdf->mlx, fdf->proj_map, fdf->rows, fdf->cols);
	mlx_hook(fdf->mlx->win, 17, 0, close_window, fdf);
	mlx_hook(fdf->mlx->win, 2, 1L << 0, key_hook, fdf);
	mlx_loop(fdf->mlx->mlx);
}

int	main(int argc, char **argv)
{
	t_fdf	fdf;

	if (argc != 2)
		return (printf("Usage: %s <fdf file>\n", argv[0]), 1);
	setup_fdf(&fdf, argv[1]);
	start_mlx(&fdf);
	return (0);
}
