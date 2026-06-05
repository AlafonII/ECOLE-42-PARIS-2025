/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:48:38 by alex              #+#    #+#             */
/*   Updated: 2025/03/25 23:14:27 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static int	check_death(t_data *data)
{
	int		i;
	long	current;

	i = 0;
	while (i < data->params.nb_philos)
	{
		current = get_time_in_ms();
		if ((current - data->philos[i].last_meal) > data->params.time_to_die)
		{
			pthread_mutex_lock(&data->print_mutex);
			printf("%ld %d died\n", current - data->start_time,
				data->philos[i].id);
			pthread_mutex_unlock(&data->print_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_meal_counts(t_data *data)
{
	int	i;
	int	all_full;

	if (data->params.nb_must_eat == -1)
		return (0);
	all_full = 1;
	i = 0;
	while (i < data->params.nb_philos)
	{
		if (data->philos[i].meals_eaten < data->params.nb_must_eat)
			all_full = 0;
		i++;
	}
	return (all_full);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (data->simulation_ended == 0)
	{
		if (check_death(data))
		{
			data->simulation_ended = 1;
			return (NULL);
		}
		if (check_meal_counts(data))
		{
			data->simulation_ended = 1;
			return (NULL);
		}
		usleep(50);
	}
	return (NULL);
}
