/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:20:43 by alex              #+#    #+#             */
/*   Updated: 2025/06/12 17:39:05 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: bfs.c
 * ---------------------------------------------------------------
 * Perform BFS from player start to ensure map is fully enclosed.
 * ---------------------------------------------------------------
 */

/*
 * Function: check_no_leak
 * Role: Perform BFS from player to detect any map leaks.
 * 
 * Parameters:
 *   map - NULL-terminated array of map row strings.
 * 
 * Returns:
 *   SUCCESS if no leaks detected; otherwise FAILURE.
 */
int	check_no_leak(char **map)
{
	t_bfs	ctx;
	int		si;
	int		sj;
	int		result;

	if (!find_player(map, &si, &sj))
		return (FAILURE);
	init_bfs_context(&ctx, map);
	result = bfs_no_leak(&ctx, si, sj);
	free_bfs_context(&ctx);
	return (result);
}
