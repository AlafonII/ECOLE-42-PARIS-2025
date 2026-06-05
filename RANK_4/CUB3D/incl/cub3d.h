/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 19:15:00 by alex              #+#    #+#             */
/*   Updated: 2025/06/12 20:14:44 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/* ************************************************************************** */
/*                                INCLUDES                                    */
/* ************************************************************************** */

# include "mlx.h"
# include "get_next_line.h"
# include "libft.h"
# include <stdbool.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <X11/keysym.h>
# include <X.h>
# include <math.h>

/* ************************************************************************** */
/*                                MACROS                                      */
/* ************************************************************************** */

enum e_output
{
	SUCCESS = 0,
	FAILURE = 1,
};

enum e_ray_direction_impact
{
	LINE = 0,
	COLUMN = 1,
};

enum e_ray_orientation_impact
{
	NORTH = 0,
	WEST = 1,
	SOUTH = 2,
	EAST = 3,
};

enum e_keys
{
	W_KEY,
	A_KEY,
	S_KEY,
	D_KEY,
	LEFT_KEY,
	RIGHT_KEY,
	KEY_COUNT
};

/* ************************************************************************** */
/*                                DEFINES                                     */
/* ************************************************************************** */

/* MENU SCREEN COLOR */
# define CYAN	"\033[0;32m"
# define RESET	"\e[0m"

/* ERROR_MESSAGES */
# define ERR_MLX_INIT		"mlx_init() failed\n"
# define ERR_WIN_CREATE		"mlx_new_window() failed\n"
# define ERR_IMG_CREATE		"mlx_new_image() failed\n"
# define ERR_USE			"Error Usage: ./cub3D <map.cub>\n"

# define MEMORY_ERROR		"Memory error"
# define TXTR_ERROR			"Invalid texture(s)"
# define DEF_ERROR			"Definition(s) error"
# define FILE_OPEN_ERROR	"Cannot open file"
# define MAP_ROW_ERROR		"Invalid map row"
# define MAP_ERROR			"Invalid map"

/* DISPLAY_PARAMETERS */
# define WIN_WIDTH 		1200
# define WIN_HEIGHT 	900
# define CELL_SIZE 		64
# define WALL_EA_COLOR		0x00FF00
# define WALL_SO_COLOR		0x0000FF
# define WALL_WE_COLOR		0xFF0000
# define WALL_NO_COLOR		0xFF00FF

/* PLAYER_PARAMETERS */
# define DIR_E	0.0
# define DIR_S	1.570796326794897
# define DIR_O	3.141592653589793
# define DIR_N	4.71238898038469
# define FOV	1.047197551196598

/* PLAYER_MOVEMENTS */
# define MOVE_SPEED 2.0
# define ROT_SPEED  0.02
# define COLLISION_BUFFER 15.0

/* ************************************************************************** */
/*                                STRUCTURES                                  */
/* ************************************************************************** */
typedef struct s_mapinfo
{
	int		fd;
	char	*path;
	char	**file;
	int		line_count;
	int		height;
	int		width;
	int		map_started;
	int		blank_after;
}	t_mapinfo;

typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	player_view;
	double	game_pos_x;
	double	game_pos_y;
}	t_player;

typedef struct s_image
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t_image;

typedef struct s_data_texture
{
	t_image	image;
	int		width;
	int		height;
	int		*data;
	int		col;
	double	row;
}	t_data_texture;

typedef struct s_textures
{
	char			*no;
	char			*we;
	char			*so;
	char			*ea;
	int				floor_color[3];
	int				ceiling_color[3];
	t_data_texture	datatex[4];
}	t_textures;

typedef struct s_data
{
	t_player	player;
	t_textures	textures;
	char		**map;
}	t_data;

typedef struct s_graphic
{
	void	*mlx;
	void	*win;
}	t_graphic;

typedef struct s_raycast
{
	int		row;
	int		col;
	int		row_init;
	int		col_init;
	int		incr_x;
	int		incr_y;
	int		delta_x;
	int		delta_y;
	int		next_x;
	int		next_y;
	int		dist;
	int		size;
	int		real_size;
	int		side_impact;
	int		pixel_col;
	int		wall_start;
	int		wall_end;
	double	angle;
}	t_raycast;

typedef struct s_game
{
	t_graphic	*graph;
	t_data		*data;
	int			key_state[KEY_COUNT];
}	t_game;

typedef struct s_bfs
{
	char	**map;
	int		rows;
	size_t	cols;
	size_t	cap;
	int		*qi;
	int		*qj;
	int		**vis;
}	t_bfs;

/* ************************************************************************** */
/*                                MAP_CHECK                                   */
/* ************************************************************************** */

/* border_check.c */
int			check_map_borders(char **map);

/* map_file_check.c */
int			map_file_check(const char *filename);

/* player_check.c */
int			check_and_set_player(char **map, t_data *data);

/* map_fill.c */
char		**replace_whitespace(char **map);

/* map_check.c */
int			is_map_row(char *line);
char		**dup_str_array(char **arr);
int			validate_map(char **map_lines, t_data *data);

/* bfs.c */
int			check_no_leak(char **map);

/* bfs_context.c */
void		init_bfs_context(t_bfs *ctx, char **map);
void		free_bfs_context(t_bfs *ctx);

/* bfs_map.c */
int			map_rows(char **map);
int			get_max_cols(char **map);
int			find_player(char **map, int *si, int *sj);
char		get_map_char(t_bfs *ctx, int i, int j);

/* bfs_utils.c */
int			bfs_no_leak(t_bfs *ctx, int si, int sj);

/* ************************************************************************** */
/*                                PARSE_DATA                                  */
/* ************************************************************************** */

/* parse_map.c */
int			parse_map_lines(t_mapinfo *info);

/* parse_texture.c */
int			parse_texture_headers(const char *trim, t_textures *textures);

/* parse_colour.c */
int			parse_color_headers(const char *trim, t_textures *textures);

/* parse_data.c */
t_data		*parse_cub_file(const char *path);
char		*trim_spaces(char *s);

/* parse_texture_utils.c */
int			check_texture_directory(const char *dir_path);
int			check_xpm_extension(const char *file_path);
int			validate_all_textures(const t_textures *textures);

/* ************************************************************************** */
/*                                FREE_DATA                                   */
/* ************************************************************************** */

/* free.c */
void		free_config(t_data *cfg);
void		free_map(char **map);
void		cleanup_mlx(t_graphic *graph);

/* error.c */
void		error_exit(const char *msg);
void		cleanup_and_exit(t_data *cfg, int fd, const char *msg);

/* ************************************************************************** */
/*                                INIT                                        */
/* ************************************************************************** */

/* init.c */
t_data		*init_data(void);

/* init_utils.c */
void		player_init(t_player *player);
void		textures_init(t_textures *tx);
void		colour_init(t_textures *tx);
void		map_init(t_data *cfg);
void		mapinfo_init(t_mapinfo *info, int fd, const char *path);

/* ************************************************************************** */
/*                                DISPLAY                                     */
/* ************************************************************************** */

/* mlx.c */
void		graphic_initializing(t_graphic *graph);
int			key_action(int keycode, void *graph);
int			close_window(void *graph);
void		image_initializing(t_graphic *graph, t_image *image,
				int width, int height);

/* gaming.c */
void		gaming(t_graphic *graph, t_data *cfg);

/* image.c */
void		raytracing(t_data *cfg, t_image *img, t_raycast *raycast);

/* textures.c */
int			rgb_to_int(int rgb[3]);
void		load_textures(t_graphic *graph, t_data *cfg);
int			find_texture_col(t_data *cfg, t_raycast *raycast);

/* raycasting.c */
void		rayswepting(t_graphic *graph, t_data *cfg);

/* ************************************************************************** */
/*                                MOVEMENT                                    */
/* ************************************************************************** */

/* player_movement.c */
int			handle_keyprs(int key, void *param);
int			handle_keyrelease(int key, void *param);
int			game_loop(void *param);

/* try_move.c */
void		try_move(t_data *d, double dx, double dy);

#endif
