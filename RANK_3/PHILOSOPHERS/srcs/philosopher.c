/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:38:14 by alex              #+#    #+#             */
/*   Updated: 2025/03/25 23:17:30 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*one_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo->data, philo->id, "has taken a fork");
	philo_sleep(philo->data, philo->data->params.time_to_die);
	print_status(philo->data, philo->id, "died");
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

void	eat_sleep_routine(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	lock_philo_forks(philo, &first, &second);
	if (philo->data->simulation_ended)
	{
		pthread_mutex_unlock(first);
		pthread_mutex_unlock(second);
		return ;
	}
	print_status(philo->data, philo->id, "is eating");
	philo->last_meal = get_time_in_ms();
	philo_sleep(philo->data, philo->data->params.time_to_eat);
	philo->meals_eaten++;
	pthread_mutex_unlock(first);
	pthread_mutex_unlock(second);
	print_status(philo->data, philo->id, "is sleeping");
	philo_sleep(philo->data, philo->data->params.time_to_sleep);
}

void	think_routine(t_philo *philo, int silent)
{
	long	time_to_think;

	time_to_think = (philo->data->params.time_to_die
			- (get_time_in_ms() - philo->last_meal)
			- philo->data->params.time_to_eat) / 2;
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (!silent)
		print_status(philo->data, philo->id, "is thinking");
	philo_sleep(philo->data, time_to_think);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->last_meal = philo->data->start_time;
	sim_start_delay(philo->data->start_time);
	if (philo->data->params.time_to_die == 0)
		return (NULL);
	if (philo->data->params.nb_philos == 1)
		return (one_philo_routine(philo));
	else if (philo->id % 2)
		think_routine(philo, 1);
	while (!philo->data->simulation_ended)
	{
		eat_sleep_routine(philo);
		think_routine(philo, 0);
	}
	return (NULL);
}
