/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:48:12 by alex              #+#    #+#             */
/*   Updated: 2025/03/25 23:16:41 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	lock_philo_forks(t_philo *philo,
			pthread_mutex_t **first,
			pthread_mutex_t **second)
{
	if (philo->left_fork < philo->right_fork)
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
	pthread_mutex_lock(*first);
	print_status(philo->data, philo->id, "has taken a fork");
	pthread_mutex_lock(*second);
	print_status(philo->data, philo->id, "has taken a fork");
}

void	print_status(t_data *data, int philo_id, char *status)
{
	long	relative_time;

	pthread_mutex_lock(&data->print_mutex);
	if (data->simulation_ended)
	{
		pthread_mutex_unlock(&data->print_mutex);
		return ;
	}
	relative_time = get_time_in_ms() - data->start_time;
	printf("%ld %d %s\n", relative_time, philo_id, status);
	pthread_mutex_unlock(&data->print_mutex);
}
