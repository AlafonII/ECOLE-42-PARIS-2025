/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:11:22 by alex              #+#    #+#             */
/*   Updated: 2025/01/31 22:41:09 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static void	store_char(unsigned char *c, char *buffer, int *pos)
{
	if (*c == '\0')
	{
		write(1, buffer, *pos);
		write(1, "\n", 1);
		*pos = 0;
	}
	else
	{
		buffer[*pos] = *c;
		(*pos)++;
	}
	*c = 0;
}

static void	handle_signal(int sig, siginfo_t *info, void *context)
{
	static unsigned char	c;
	static int				bits;
	static char				buffer[100000];
	static int				pos;

	(void)context;
	if (sig == SIGUSR1)
		c |= (1 << bits);
	bits++;
	if (bits == 8)
	{
		store_char(&c, buffer, &pos);
		bits = 0;
	}
	kill(info->si_pid, SIGUSR1);
	usleep(100);
}

int	main(void)
{
	struct sigaction	sa;

	ft_putstr("Server PID: ");
	ft_putnbr(getpid());
	ft_putstr("\n");
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = &handle_signal;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		pause();
	return (0);
}
