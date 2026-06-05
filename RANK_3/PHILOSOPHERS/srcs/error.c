/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:49:59 by alex              #+#    #+#             */
/*   Updated: 2025/03/25 23:13:56 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	print_error(char *msg)
{
	int	len;

	len = 0;
	while (msg[len] != '\0')
		len++;
	write(2, msg, len);
}

void	free_resources(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->params.nb_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	pthread_mutex_destroy(&data->print_mutex);
	if (data->philos)
		free(data->philos);
}

int	validate_args(int argc, char **argv)
{
	int	i;
	int	j;
	int	value;

	i = 1;
	while (i < argc)
	{
		if (!argv[i] || argv[i][0] == '\0')
			return (print_error("Error: Empty argument\n"), 1);
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (print_error("All args must be positive integers\n"), 1);
			j++;
		}
		value = ft_atoi(argv[i]);
		if (value <= 0)
			return (print_error("All args must be greater than 0\n"), 1);
		i++;
	}
	return (0);
}
