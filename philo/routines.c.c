/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:29:34 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/08 16:59:16 by elara-va         ###   ########.fr       */
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
			>= resources->time_to_die) // Or all meals have been had
		{
			resources->stop_flag = 1;
			// Locking print_lock might delay the printing of a death, I believe
			printf("%ldms %d died\n",
				get_time_interval_ms(resources->initial_time, current_time), curr_philo_node->philosopher);
			break ;
		}
		if (curr_philo_node->next != NULL)
			curr_philo_node = curr_philo_node->next;
		else
			curr_philo_node = resources->philosopher_list;
	}
	return (NULL);
}

// Here goes the loop function
static void	do_tasks(t_resources *resources, t_philosopher_list *philosopher_node)
{
	int	meals_had;
	
	meals_had = 0;
	// START HERE
	while (resources->stop_flag == 0)
	{
	// Eating
	// Thinking
	// Sleeping
	}
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
	if (resources->nbr_of_meals == -1)
		do_tasks(resources, philosopher_node);
	else
	{
		// Same function but each philosopher increments a meals_had variable (local to each thread)
		// every time they eat. After a philosopher has had all his meals,
		// he raises the full_philos_flag (but keeps running).
		// When full_philos_flag is equal to requested_philos, the monitor raises stop_flag.
	}
	
	return (NULL);
}
