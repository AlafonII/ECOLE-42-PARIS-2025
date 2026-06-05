/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:30:44 by alafon            #+#    #+#             */
/*   Updated: 2025/06/05 11:41:05 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: player_movement.c
 * ---------------------------------------------------------------
 * Handle player movement, key events, and main game loop rendering.
 * ---------------------------------------------------------------
 */

/*
 * Function: update_key_state
 * Role: Update game key_state array for given key and value.
 * 
 * Parameters:
 *   key   - Key code from event (e.g., XK_w, XK_Left).
 *   value - 1 for press, 0 for release.
 *   game  - Pointer to t_game containing key_state.
 */
static void	update_key_state(int key, int value, t_game *game)
{
	if (key == XK_w)
		game->key_state[W_KEY] = value;
	else if (key == XK_s)
		game->key_state[S_KEY] = value;
	else if (key == XK_d)
		game->key_state[D_KEY] = value;
	else if (key == XK_a)
		game->key_state[A_KEY] = value;
	else if (key == XK_Left)
		game->key_state[LEFT_KEY] = value;
	else if (key == XK_Right)
		game->key_state[RIGHT_KEY] = value;
}

/*
 * Function: poll_keys
 * Role: Check current key_state and move or rotate player accordingly.
 * 
 * Parameters:
 *   game - Pointer to t_game containing input state and data.
 */
static void	poll_keys(t_game *game)
{
	t_data	*d;
	double	dir;

	d = game->data;
	dir = d->player.player_view;
	if (game->key_state[W_KEY])
		try_move(d,
			cos(dir) * MOVE_SPEED,
			sin(dir) * MOVE_SPEED);
	if (game->key_state[S_KEY])
		try_move(d,
			-cos(dir) * MOVE_SPEED,
			-sin(dir) * MOVE_SPEED);
	if (game->key_state[D_KEY])
		try_move(d,
			cos(dir + M_PI_2) * MOVE_SPEED,
			sin(dir + M_PI_2) * MOVE_SPEED);
	if (game->key_state[A_KEY])
		try_move(d,
			cos(dir - M_PI_2) * MOVE_SPEED,
			sin(dir - M_PI_2) * MOVE_SPEED);
	if (game->key_state[LEFT_KEY])
		d->player.player_view -= ROT_SPEED;
	if (game->key_state[RIGHT_KEY])
		d->player.player_view += ROT_SPEED;
}

/*
 * Function: game_loop
 * Role: Perform one frame: poll keys, clear window, render scene.
 * 
 * Parameters:
 *   param - Pointer to t_game passed as void*.
 * 
 * Returns:
 *   SUCCESS always.
 */
int	game_loop(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	poll_keys(game);
	rayswepting(game->graph, game->data);
	return (SUCCESS);
}

/*
 * Function: handle_keypress
 * Role: On key press, close window on Escape or set key state.
 * 
 * Parameters:
 *   key   - Key code from event.
 *   param - Pointer to t_game passed as void*.
 * 
 * Returns:
 *   SUCCESS always.
 */
int	handle_keyprs(int key, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (key == XK_Escape)
		close_window(param);
	else
		update_key_state(key, 1, game);
	return (SUCCESS);
}

/*
 * Function: handle_keyrelease
 * Role: On key release, update key state accordingly.
 * 
 * Parameters:
 *   key   - Key code from event.
 *   param - Pointer to t_game passed as void*.
 * 
 * Returns:
 *   SUCCESS always.
 */
int	handle_keyrelease(int key, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	update_key_state(key, 0, game);
	return (SUCCESS);
}
