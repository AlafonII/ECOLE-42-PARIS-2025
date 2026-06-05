/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:10:25 by alex              #+#    #+#             */
/*   Updated: 2025/06/06 13:05:35 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	print_controls(void)
{
	printf(CYAN "\n");
	printf("      ::::::::  :::    ::: :::::::::      ::::::::  ::::::::: \n");
	printf("    :+:    :+: :+:    :+: :+:    :+:    :+:    :+: :+:    :+: \n");
	printf("   +:+        +:+    +:+ +:+    +:+           +:+ +:+    +:+  \n");
	printf("  +#+        +#+    +:+ +#+    +:+        +#++:  +#+    +:+   \n");
	printf(" +#+        +#+    +#+ +#+    +#+           +#+ +#+    +#+    \n");
	printf("#+#    #+# #+#    #+# #+#    #+#    #+#    #+# #+#    #+#     \n");
	printf("########   ########  #########      ########  #########       \n");
	printf(RESET "\n");
	printf(CYAN "\tW" RESET ": move forward\t");
	printf(CYAN "\tS" RESET ": move backward\n");
	printf(CYAN "\tA" RESET ": strafe left\t");
	printf(CYAN "\tD" RESET ": strafe right\n");
	printf(CYAN "\t<" RESET ": rotate left\t");
	printf(CYAN "\t>" RESET ": rotate right\n");
	printf("\n");
}

int	main(int argc, char **argv)
{
	t_graphic	graph;
	t_data		*cfg;
	t_game		game;
	int			i;

	if (argc != 2)
		return (ft_putstr_fd(ERR_USE, STDERR_FILENO), EXIT_FAILURE);
	cfg = parse_cub_file(argv[1]);
	if (!cfg)
		return (EXIT_FAILURE);
	print_controls();
	graphic_initializing(&graph);
	load_textures(&graph, cfg);
	rayswepting(&graph, cfg);
	game.graph = &graph;
	game.data = cfg;
	i = 0;
	while (i < KEY_COUNT)
		game.key_state[i++] = 0;
	mlx_hook(graph.win, KeyPress, KeyPressMask, (void *)handle_keyprs, &game);
	mlx_hook(graph.win, KeyRelease, KeyReleaseMask,
		(void *)handle_keyrelease, &game);
	mlx_loop_hook(graph.mlx, game_loop, &game);
	mlx_hook(graph.win, 17, 0, (void *)close_window, &game);
	return (mlx_loop(graph.mlx), EXIT_SUCCESS);
}
