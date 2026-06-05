/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 19:52:34 by alex              #+#    #+#             */
/*   Updated: 2025/06/12 20:14:52 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: init_utils.c
 * ---------------------------------------------------------------
 * Initialize t_data struct with default values and allocate memory.
 * ---------------------------------------------------------------
 */

/*
 * Function: player_init
 * Role: Set player position to (0,0).
 *
 * Parameters:
 *   player - Pointer to t_player struct.
 */
void	player_init(t_player *player)
{
	player->pos_x = 0;
	player->pos_y = 0;
	player->player_view = DIR_E;
	player->game_pos_x = 0;
	player->game_pos_y = 0;
}

/*
 * Function: textures_init
 * Role: Set all texture pointers in t_textures to NULL.
 *
 * Parameters:
 *   tx - Pointer to t_textures struct.
 */
void	textures_init(t_textures *tx)
{
	tx->no = NULL;
	tx->so = NULL;
	tx->we = NULL;
	tx->ea = NULL;
}

/*
 * Function: colour_init
 * Role: Initialize floor and ceiling color arrays to zero.
 *
 * Parameters:
 *   tx - Pointer to t_textures struct.
 */
void	colour_init(t_textures *tx)
{
	tx->floor_color[0] = 0;
	tx->floor_color[1] = 0;
	tx->floor_color[2] = 0;
	tx->ceiling_color[0] = 0;
	tx->ceiling_color[1] = 0;
	tx->ceiling_color[2] = 0;
}

/*
 * Function: map_init
 * Role: Set the map pointer in t_data to NULL.
 *
 * Parameters:
 *   cfg - Pointer to t_data struct.
 */
void	map_init(t_data *cfg)
{
	cfg->map = NULL;
}

/*
 * Function: mapinfo_init
 * Role: Initialize t_mapinfo struct before parsing map lines.
 *
 * Parameters:
 *   info - Pointer to t_mapinfo struct to initialize.
 *   fd   - File descriptor for map file.
 *   path - Path to .cub configuration file.
 */
void	mapinfo_init(t_mapinfo *info, int fd, const char *path)
{
	info->fd = fd;
	info->path = (char *)path;
	info->file = NULL;
	info->line_count = 0;
	info->height = 0;
	info->width = 0;
	info->map_started = 0;
	info->blank_after = 0;
}
