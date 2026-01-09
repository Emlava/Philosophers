/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:29:34 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/09 14:34:07 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_resources			*resources;
	t_philosopher_list	*curr_philo_node;
	struct timeval		current_time;
	
	resources = (t_resources*)arg;
	curr_philo_node = resources->philosopher_list;
	while (1)
	{
		while (curr_philo_node->prev_meal_or_initial_ts.tv_sec == -1)
		{
			if (resources->error_creating_thread_flag)
			{
				resources->stop_flag = 1;
				return (NULL) ;
			}
		}
		gettimeofday(&current_time, NULL);
		if (get_time_interval_ms(curr_philo_node->prev_meal_or_initial_ts, current_time)
			>= resources->time_to_die)
		{
			resources->stop_flag = 1;
			pthread_mutex_lock(&resources->print_lock);
			printf("%ldms %d died\n",
				get_time_interval_ms(resources->initial_time, current_time), curr_philo_node->philosopher);
			pthread_mutex_unlock(&resources->print_lock);
			break ;
		}
		if (resources->full_philos_flag == resources->nbr_of_meals)
		{
			resources->stop_flag = 1;
			break ;
		}
		if (curr_philo_node->next != NULL)
			curr_philo_node = curr_philo_node->next;
		else
			curr_philo_node = resources->philosopher_list;
	}
	return (NULL);
}

static void	do_tasks(t_resources *resources, t_philosopher_list *philosopher_node)
{
	int				meals_had;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct timeval	curr_time;
	
	meals_had = 0;
	if (philosopher_node->philosopher == 1)
		left_fork = &resources->forks[resources->requested_philos - 1];
	else if (philosopher_node->philosopher == resources->requested_philos)
		right_fork = &resources->forks[0];
	if (philosopher_node->philosopher > 1)
		left_fork = &resources->forks[philosopher_node->philosopher - 2];
	if (philosopher_node->philosopher < resources->requested_philos)
		right_fork = &resources->forks[philosopher_node->philosopher - 1];

	while (1)
	{
		// Eating
		// Check if there can be a deadlock with the forks
		pthread_mutex_lock(left_fork);
		if (!print_state_change(resources, TF, philosopher_node))
			break ;
		pthread_mutex_lock(right_fork);
		if (!print_state_change(resources, TF, philosopher_node))
			break ;
		if (!print_state_change(resources, E, philosopher_node))
			break ;
		meals_had++;
		if (meals_had == resources->nbr_of_meals)
			resources->full_philos_flag++;
		gettimeofday(&curr_time, NULL);
		while (get_time_interval_ms(philosopher_node->prev_meal_or_initial_ts, curr_time) < resources->time_to_eat)
			gettimeofday(&curr_time, NULL);
		pthread_mutex_unlock(left_fork);
		pthread_mutex_unlock(right_fork);
		
		// Sleeping
		if (!print_state_change(resources, S, philosopher_node))
			break ;
		usleep(resources->time_to_sleep * 1000);

		// Thinking
		if (!print_state_change(resources, T, philosopher_node))
			break ;
	}
	return ;
}

void	*philosophers_routine(void *arg)
{
	t_resources			*resources;
	int					philosopher_nbr;
	t_philosopher_list	*philosopher_node;
	int					i;
	
	resources = (t_resources*)arg;
	pthread_mutex_lock(&resources->philo_nbr_lock);
	philosopher_nbr = ++resources->seat_nbr;
	pthread_mutex_unlock(&resources->philo_nbr_lock);
	philosopher_node = resources->philosopher_list;
	i = 0;
	while (++i != philosopher_nbr)
		philosopher_node = philosopher_node->next;
	philosopher_node->philosopher = philosopher_nbr;
	philosopher_node->prev_meal_or_initial_ts = resources->initial_time;
	do_tasks(resources, philosopher_node);
	return (NULL);
}
