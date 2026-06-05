/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:20:24 by alex              #+#    #+#             */
/*   Updated: 2025/06/12 18:16:51 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: bfs_utils.c
 * ---------------------------------------------------------------
 * Check bounds, enqueue neighbors, and BFS leak detection utilities.
 * ---------------------------------------------------------------
 */

/*
 * Function: is_out_of_bounds
 * Role: Check if row or column index is outside map bounds.
 * 
 * Parameters:
 *   ctx - BFS context with map and dimensions.
 *   ni  - Row index to check.
 *   nj  - Column index to check.
 * 
 * Returns:
 *   1 if out of bounds; otherwise 0.
 */
static int	is_out_of_bounds(t_bfs *ctx, int ni, int nj)
{
	if (ni < 0 || ni >= ctx->rows)
		return (1);
	if ((size_t)nj >= ft_strlen(ctx->map[ni]))
		return (1);
	return (0);
}

/*
 * Function: mark_and_enqueue
 * Role: Mark position visited and add it to the BFS queue.
 * 
 * Parameters:
 *   ctx  - BFS context with visited array and queues.
 *   ni   - Row index to enqueue.
 *   nj   - Column index to enqueue.
 *   rear - Pointer to queue rear index.
 */
static void	mark_and_enqueue(t_bfs *ctx, int ni, int nj, int *rear)
{
	ctx->vis[ni][nj] = 1;
	ctx->qi[*rear] = ni;
	ctx->qj[*rear] = nj;
	(*rear)++;
}

/*
 * Function: process_direction
 * Role: Enqueue valid unvisited neighbor if it is not a wall.
 * 
 * Parameters:
 *   ctx  - BFS context with map and visited flags.
 *   ni   - Neighbor row index.
 *   nj   - Neighbor column index.
 *   rear - Pointer to queue rear index.
 * 
 * Returns:
 *   SUCCESS if processed; FAILURE if out of bounds.
 */
static int	process_direction(t_bfs *ctx, int ni, int nj, int *rear)
{
	char	c;

	if (is_out_of_bounds(ctx, ni, nj))
		return (FAILURE);
	c = ctx->map[ni][nj];
	if (c != '1' && !ctx->vis[ni][nj])
		mark_and_enqueue(ctx, ni, nj, rear);
	return (SUCCESS);
}

/*
 * Function: check_neighbors
 * Role: Check and enqueue neighbors in all four directions.
 * 
 * Parameters:
 *   ctx  - BFS context with map and queues.
 *   ci   - Current row index.
 *   cj   - Current column index.
 *   rear - Pointer to queue rear index.
 * 
 * Returns:
 *   SUCCESS if all neighbors processed; FAILURE on error.
 */
static int	check_neighbors(t_bfs *ctx, int ci, int cj, int *rear)
{
	int		k;
	int		di[4];
	int		dj[4];
	int		ni;
	int		nj;

	di[0] = -1;
	dj[0] = 0;
	di[1] = 1;
	dj[1] = 0;
	di[2] = 0;
	dj[2] = -1;
	di[3] = 0;
	dj[3] = 1;
	k = 0;
	while (k < 4)
	{
		ni = ci + di[k];
		nj = cj + dj[k];
		if (process_direction(ctx, ni, nj, rear) == FAILURE)
			return (FAILURE);
		k++;
	}
	return (SUCCESS);
}

/*
 * Function: bfs_no_leak
 * Role: Perform BFS from start to detect any map leaks.
 * 
 * Parameters:
 *   ctx - BFS context with map, visited flags, and queues.
 *   si  - Start row index.
 *   sj  - Start column index.
 * 
 * Returns:
 *   SUCCESS if no leaks; otherwise FAILURE.
 */
int	bfs_no_leak(t_bfs *ctx, int si, int sj)
{
	int	front;
	int	rear;
	int	ci;
	int	cj;

	front = 0;
	rear = 0;
	ctx->qi[rear] = si;
	ctx->qj[rear] = sj;
	ctx->vis[si][sj] = 1;
	rear++;
	while (front < rear)
	{
		ci = ctx->qi[front];
		cj = ctx->qj[front];
		front++;
		if (check_neighbors(ctx, ci, cj, &rear) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}
