/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:41:52 by alex              #+#    #+#             */
/*   Updated: 2025/03/25 23:18:49 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

/* ************************************************************************** */
/*                                INCLUDES                                    */
/* ************************************************************************** */

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

/* ************************************************************************** */
/*                                STRUCTURES                                  */
/* ************************************************************************** */

typedef struct s_params
{
	int		nb_philos;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	int		nb_must_eat;
}	t_params;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	t_params		params;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	int				simulation_ended;
	long			start_time;
}	t_data;

/* ************************************************************************** */
/*                              INITIALIZATION                                */
/* ************************************************************************** */

int		init_params(t_data *data, int argc, char **argv);
int		init_philosophers(t_data *data);
int		init_mutexes(t_data *data);

/* ************************************************************************** */
/*                             FORK MANAGEMENT                                */
/* ************************************************************************** */

void	lock_philo_forks(t_philo *philo,
			pthread_mutex_t **first,
			pthread_mutex_t **second);

/* ************************************************************************** */
/*                             SIMULATION LOOP                                */
/* ************************************************************************** */

void	*philosopher_routine(void *arg);
void	*monitor_routine(void *arg);

/* ************************************************************************** */
/*                                 UTILITIES                                  */
/* ************************************************************************** */

long	get_time_in_ms(void);
void	sleep_ms(long ms);
void	philo_sleep(t_data *data, long sleep_time);
void	print_status(t_data *data, int philo_id, char *status);
int		ft_atoi(const char *str);
void	print_error(char *msg);
void	free_resources(t_data *data);
int		validate_args(int argc, char **argv);
void	sim_start_delay(long start_time);

#endif
