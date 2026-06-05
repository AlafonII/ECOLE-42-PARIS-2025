/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:17:40 by alafon            #+#    #+#             */
/*   Updated: 2025/05/29 22:49:16 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: error.c
 * ---------------------------------------------------------------
 * Print error message to stderr and exit with failure status.
 * ---------------------------------------------------------------
 */

/*
 * Function: error_exit
 * Role: Write "Error", the message, and newline to stderr, then exit.
 *
 * Parameters:
 *   msg - Null-terminated error message string.
 */
void	error_exit(const char *msg)
{
	write(2, "Error : ", 8);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(EXIT_FAILURE);
}

void	cleanup_and_exit(t_data *cfg, int fd, const char *msg)
{
	if (cfg)
		free_config(cfg);
	if (fd >= 0)
		close(fd);
	error_exit(msg);
}
