/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 20:21:24 by alex              #+#    #+#             */
/*   Updated: 2025/02/24 09:39:56 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

int	key_hook(int keycode, void *param)
{
	t_fdf	*fdf;

	fdf = (t_fdf *)param;
	(void)param;
	if (keycode == 27 || keycode == 53 || keycode == 65307)
	{
		cleanup_all(fdf);
		exit(0);
	}
	return (0);
}

int	close_window(void *param)
{
	t_fdf	*fdf;

	fdf = (t_fdf *)param;
	cleanup_all(fdf);
	exit(0);
	return (0);
}
