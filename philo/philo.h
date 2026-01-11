/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:00:12 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/11 14:33:52 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# define TF "has taken a fork"
# define E "is eating"
# define S "is sleeping"
# define T "is thinking"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_philosopher_list
{
	pthread_t					thread;
	int							philosopher;
	struct timeval				prev_meal_or_initial_ts; // Protect
	pthread_mutex_t 			pmits_lock;
	struct s_philosopher_list	*next;
}	t_philosopher_list;

typedef struct s_resources
{
	t_philosopher_list	*philosopher_list;
	int					requested_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nbr_of_meals;
	int					seat_nbr;
	struct timeval		initial_time;
	int					error_creating_thread_flag;
	int					stop_flag;
	int					full_philos_flag;
	pthread_mutex_t	 	*forks;
	pthread_mutex_t 	philo_nbr_lock;
	pthread_mutex_t 	print_lock;
	pthread_mutex_t		ect_flag_lock;
	pthread_mutex_t		stop_flag_lock;
	pthread_mutex_t		fp_flag_lock;
}	t_resources;

/**** utilities/setup_utils.c ****/
int		convert_args_to_int(char *av[], t_resources *resources, int ac);
int		allocate_philos_list(t_philosopher_list **philosopher_list, int requested_philos);
int		create_forks(t_resources *resources);
int		create_locks(t_resources *resources);

/**** utilities/miscellaneous_utils.c ****/
int		ft_atoi(const char *nptr);
long	get_time_interval_ms(struct timeval first_ts, struct timeval second_ts);
int		print_state_change(t_resources *resources, char *new_state,
			t_philosopher_list *philosopher_node);

/**** utilities/cleaning_utils.c ****/
void	free_philos_list(t_philosopher_list *philosopher_list);
void	destroy_forks(pthread_mutex_t *forks, int nbr_of_forks);

/**** routines.c ****/
void	*monitor_routine(void *arg);
void	*philosophers_routine(void *arg);

#endif