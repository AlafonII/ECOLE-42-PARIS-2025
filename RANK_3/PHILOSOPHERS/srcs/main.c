/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:46:57 by alex              #+#    #+#             */
/*   Updated: 2025/03/25 23:14:18 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static int	init_simulation(t_data *data, int argc, char **argv)
{
	int	i;

	if (validate_args(argc, argv))
		return (1);
	if (init_params(data, argc, argv))
		return (1);
	if (init_mutexes(data))
	{
		free_resources(data);
		return (1);
	}
	if (init_philosophers(data))
	{
		free_resources(data);
		return (1);
	}
	data->start_time = get_time_in_ms() + (data->params.nb_philos * 2 * 10);
	i = 0;
	while (i < data->params.nb_philos)
	{
		data->philos[i].last_meal = data->start_time;
		i++;
	}
	return (0);
}

static int	start_threads(t_data *data, pthread_t *monitor_thread)
{
	int	i;

	i = 0;
	while (i < data->params.nb_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, (void *)&data->philos[i]) != 0)
			return (print_error("Failed to create philosopher thread"), 1);
		i++;
	}
	if (data->params.nb_philos > 1)
	{
		if (pthread_create(monitor_thread, NULL,
				monitor_routine, (void *)data) != 0)
			return (print_error("Failed to create monitor thread"), 1);
	}
	return (0);
}

static void	join_threads(t_data *data, pthread_t monitor_thread)
{
	int	i;

	if (data->params.nb_philos > 1)
		pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < data->params.nb_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

static int	run_simulation(t_data *data)
{
	pthread_t	monitor_thread;

	if (start_threads(data, &monitor_thread))
		return (1);
	join_threads(data, monitor_thread);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
	{
		print_error("Usage: nb_philos ttd tte tts [nb_eat]\n");
		return (1);
	}
	data.simulation_ended = 0;
	if (init_simulation(&data, argc, argv))
		return (1);
	if (run_simulation(&data))
	{
		free_resources(&data);
		return (1);
	}
	free_resources(&data);
	return (0);
}
