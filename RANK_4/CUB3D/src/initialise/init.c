/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 19:31:13 by alex              #+#    #+#             */
/*   Updated: 2025/06/03 12:33:43 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: init.c
 * ---------------------------------------------------------------
 * Initialize t_data struct with default values and allocate memory.
 * ---------------------------------------------------------------
 */

/*
 * Function: init_data
 * Role: Allocate and initialize t_data struct.
 *
 * Returns:
 *   Pointer to initialized t_data, or exits on allocation failure.
 */
t_data	*init_data(void)
{
	t_data	*cfg;

	cfg = ft_calloc(1, sizeof(*cfg));
	if (!cfg)
		error_exit(MEMORY_ERROR);
	player_init(&cfg->player);
	textures_init(&cfg->textures);
	colour_init(&cfg->textures);
	map_init(cfg);
	return (cfg);
}
