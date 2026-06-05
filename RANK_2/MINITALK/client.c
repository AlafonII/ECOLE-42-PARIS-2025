/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:11:35 by alex              #+#    #+#             */
/*   Updated: 2025/01/30 19:46:46 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static volatile int	g_ack_received = 0;

static void	ack_handler(int sig)
{
	(void)sig;
	g_ack_received = 1;
}

static void	send_char(char c, pid_t srv_pid)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		g_ack_received = 0;
		if (c & (1 << i))
			kill(srv_pid, SIGUSR1);
		else
			kill(srv_pid, SIGUSR2);
		while (!g_ack_received)
			pause();
		i++;
	}
}

int	main(int argc, char **argv)
{
	pid_t	server_pid;
	int		i;

	i = 0;
	if (argc != 3)
		return (ft_putstr("Usage: ./client <SERVER_PID> <MESSAGE>\n"), 1);
	signal(SIGUSR1, ack_handler);
	server_pid = ft_atoi(argv[1]);
	while (argv[2][i])
		send_char(argv[2][i++], server_pid);
	send_char('\0', server_pid);
	return (0);
}
