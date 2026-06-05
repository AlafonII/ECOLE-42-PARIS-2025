/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:31:23 by alex              #+#    #+#             */
/*   Updated: 2025/02/24 09:40:16 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# define ISO_ANGLE 0.523599
# define WIN_WIDTH 800
# define WIN_HEIGHT 800

// =======================
//       INCLUDES
// =======================

# include "libraries/minilibx-linux/mlx.h"
# include "libraries/libft/libft.h"

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <math.h>

// =======================
//       STURCTURES
// =======================

typedef struct s_point3d
{
	int	x;
	int	y;
	int	z;
}	t_point3d;

typedef struct s_point2d
{
	int	x;
	int	y;
}	t_point2d;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_img;

typedef struct s_mlx
{
	void	*mlx;
	void	*win;
	t_img	*img;
}	t_mlx;

typedef struct s_line
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
}	t_line;

typedef struct s_fdf
{
	t_mlx		*mlx;
	t_point3d	**map;
	t_point2d	**proj_map;
	int			rows;
	int			cols;
}	t_fdf;

typedef struct s_projection
{
	double		min_x;
	double		max_x;
	double		min_y;
	double		max_y;
	double		scale;
	int			x_off;
	int			y_off;
	t_point3d	**map;
	t_point2d	**proj;
	int			rows;
	int			cols;
}	t_projection;

// =======================
//       FUNCTIONS
// =======================

//================fdf_read.c===========================//
int			is_line_empty(const char *line);
int			count_columns(const char *line);
t_point3d	*parse_line(char *line, int y, int cols);

//================parse_fdf.c==========================//
t_point3d	**parse_fdf(char *filename, int *rows, int *cols);

//================iso_projection.c=====================//
void		init_bounds(t_projection *p);
void		project_map(t_point3d **map, t_point2d **proj, int rows, int cols);
t_point2d	project_iso_custom(t_point3d point, double scale,
				int x_offset, int y_offset);

//================iso_projection_utils.c===============//
void		apply_projection(t_projection *p);
void		compute_offsets(t_projection *p);
void		compute_scale(t_projection *p);
void		parse_map_bounds(t_projection *p);
void		update_xy_bounds(t_projection *p, double iso_x, double iso_y);

//================init_mlx=============================//
t_mlx		*init_mlx(int width, int height);

//================draw.c===============================//
void		draw_map(t_mlx *mlx, t_point2d **map, int rows, int cols);

//================hooks.c==============================//
int			key_hook(int keycode, void *param);
int			close_window(void *param);

//================cleanup.c============================//
void		free_map(t_point3d **map, int rows);
void		free_proj_map(t_point2d **proj_map, int rows);
void		free_split(char **split);
void		cleanup_all(t_fdf *fdf);
void		cleanup_mlx(t_fdf *fdf);

//================error.c==============================//
int			check_line_count(char **tokens, int expected);

#endif