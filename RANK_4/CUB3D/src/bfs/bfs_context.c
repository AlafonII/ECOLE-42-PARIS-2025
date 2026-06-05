/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs_context.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:19:31 by alex              #+#    #+#             */
/*   Updated: 2025/06/12 18:09:04 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: bfs_context.c
 * ---------------------------------------------------------------
 * Allocate and free BFS queues and visited matrix for leak checks.
 * ---------------------------------------------------------------
 */

/*
 * Function: alloc_bfs_queue
 * Role: Allocate and initialize BFS index queues.
 * 
 * Parameters:
 *   ctx - Pointer to BFS context with capacity set.
 */
static void	alloc_bfs_queue(t_bfs *ctx)
{
	size_t	i;

	ctx->qi = malloc(ctx->cap * sizeof * ctx->qi);
	if (!ctx->qi)
		exit(EXIT_FAILURE);
	ctx->qj = malloc(ctx->cap * sizeof * ctx->qj);
	if (!ctx->qj)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < ctx->cap)
	{
		ctx->qi[i] = 0;
		ctx->qj[i] = 0;
		i++;
	}
}

/*
 * Function: init_vis_matrix
 * Role: Allocate and initialize visitation matrix.
 * 
 * Parameters:
 *   ctx - Pointer to BFS context with rows, cols, and queues.
 */
static void	init_vis_matrix(t_bfs *ctx)
{
	size_t	i;
	size_t	j;

	ctx->vis = malloc((size_t)ctx->rows * sizeof * ctx->vis);
	if (!ctx->vis)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < (size_t)ctx->rows)
	{
		ctx->vis[i] = malloc((size_t)ctx->cols * sizeof * ctx->vis[i]);
		if (!ctx->vis[i])
			exit(EXIT_FAILURE);
		j = 0;
		while (j < (size_t)ctx->cols)
		{
			ctx->vis[i][j] = 0;
			j++;
		}
		i++;
	}
}

/*
 * Function: init_bfs_context
 * Role: Initialize BFS context dimensions and allocate resources.
 * 
 * Parameters:
 *   ctx - Pointer to BFS context to set up.
 *   map - NULL-terminated array of map row strings.
 */
void	init_bfs_context(t_bfs *ctx, char **map)
{
	ctx->map = map;
	ctx->rows = map_rows(map);
	ctx->cols = get_max_cols(map);
	ctx->cap = (size_t)ctx->rows * (size_t)ctx->cols;
	alloc_bfs_queue(ctx);
	init_vis_matrix(ctx);
}

/*
 * Function: free_bfs_context
 * Role: Free BFS context queues and visited matrix.
 * 
 * Parameters:
 *   ctx - Pointer to BFS context to clean up.
 */
void	free_bfs_context(t_bfs *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->rows)
	{
		free(ctx->vis[i]);
		i++;
	}
	free(ctx->vis);
	free(ctx->qi);
	free(ctx->qj);
}
