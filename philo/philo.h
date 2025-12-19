/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 21:18:57 by elara-va          #+#    #+#             */
/*   Updated: 2025/12/19 13:47:53 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# define TF "has taken a fork"
# define E "is eating"
# define S "is sleeping"
# define T "is thinking"
# define D "died"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_resources
{
	int				requested_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nbr_of_meals;
	pthread_mutex_t *forks;
	pthread_mutex_t philo_nbr_lock;
	pthread_mutex_t print_lock;
	int				philo_nbr;
	struct timeval	initial_time;
}	t_resources;

typedef struct s_philosophers
{
	pthread_t				thread;
	struct s_philosophers	*next;
}	t_philosophers;

/**** utilities/setup_utils.c ****/
int		convert_args_to_int(char *av[], t_resources *resources, int ac);
int		create_forks(t_resources *resources);
int		create_locks(t_resources *resources);
int		allocate_philos_list(t_philosophers **philosophers, int requested_philos);

/**** utilities/miscellaneous_utils.c ****/
int		ft_atoi(const char *nptr);
void	print_state_change(struct timeval initial_time, int philosopher, char *new_state, pthread_mutex_t *print_lock);

/**** utilities/cleaning_utils.c ****/
void	destroy_forks(pthread_mutex_t *forks, int nbr_of_forks);
void	free_philos_list(t_philosophers *philosophers);

#endif