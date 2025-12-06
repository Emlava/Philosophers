/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 21:18:57 by elara-va          #+#    #+#             */
/*   Updated: 2025/12/06 19:26:11 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# define F 1 // Forks
# define FT 2 // Forks and threads
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_resources
{
	int	nbr_of_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	nbr_of_meals;
	pthread_mutex_t *forks;
}	t_resources;

typedef struct s_threads
{
	pthread_t			thread;
	struct s_threads	*next;
	struct s_threads	*prev;
}	t_threads;

/**** atoi.c ****/
int	ft_atoi(const char *nptr);

/**** cleaning_utils.c ****/
void	destroy_forks(pthread_mutex_t *forks, int nbr_of_forks);
void	free_thread_list(t_threads *threads);

#endif