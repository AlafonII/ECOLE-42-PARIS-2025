/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:47:37 by alex              #+#    #+#             */
/*   Updated: 2025/03/25 23:17:45 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	init_params(t_data *data, int argc, char **argv)
{
	int	i;
	int	checker;

	i = 1;
	if (argc < 5 || argc > 6)
		return (print_error("Too many or too few args\n"), 1);
	while (i < argc)
	{
		checker = ft_atoi(argv[i]);
		if (checker < 0)
			return (print_error("All args must be positive integers\n"), 1);
		i++;
	}
	data->params.nb_philos = ft_atoi(argv[1]);
	data->params.time_to_die = ft_atoi(argv[2]);
	data->params.time_to_eat = ft_atoi(argv[3]);
	data->params.time_to_sleep = ft_atoi(argv[4]);
	data->params.nb_must_eat = -1;
	if (argc == 6)
		data->params.nb_must_eat = ft_atoi(argv[5]);
	return (0);
}

int	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	data->philos = malloc(sizeof(t_philo) * data->params.nb_philos);
	if (!data->philos)
		return (print_error("Error allocating memory for philosphers\n"), 1);
	while (i < data->params.nb_philos)
	{
		data->philos[i].id = (i + 1);
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal = data->start_time;
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork
			= &data->forks[(i + 1) % data->params.nb_philos];
		i++;
	}
	return (0);
}

static int	init_fork_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->params.nb_philos);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->params.nb_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (i-- > 0)
				pthread_mutex_destroy(&data->forks[i]);
			free(data->forks);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	init_print_mutex_wrapper(t_data *data)
{
	int	i;

	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		i = 0;
		while (i < data->params.nb_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
		return (1);
	}
	return (0);
}

int	init_mutexes(t_data *data)
{
	if (init_fork_mutexes(data))
		return (1);
	if (init_print_mutex_wrapper(data))
		return (1);
	return (0);
}
