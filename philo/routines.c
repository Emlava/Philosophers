/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:29:34 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/18 15:23:45 by elara-va         ###   ########.fr       */
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
		while (!pthread_mutex_lock(&curr_philo_node->pmits_lock) && curr_philo_node->prev_meal_or_initial_ts.tv_sec == -1)
		{
			pthread_mutex_unlock(&curr_philo_node->pmits_lock);
			pthread_mutex_lock(&resources->ect_flag_lock);
			if (resources->error_creating_thread_flag)
			{
				pthread_mutex_unlock(&resources->ect_flag_lock);
				pthread_mutex_lock(&resources->stop_flag_lock);
				resources->stop_flag = 1;
				pthread_mutex_unlock(&resources->stop_flag_lock);
				return (NULL) ;
			}
			pthread_mutex_unlock(&resources->ect_flag_lock);
		}
		// We don't unlock pmits_lock here because it's used in the next
		// if statement
		gettimeofday(&current_time, NULL);
		if (get_time_interval_ms(curr_philo_node->prev_meal_or_initial_ts, current_time)
			>= resources->time_to_die)
		{
			pthread_mutex_unlock(&curr_philo_node->pmits_lock);
			pthread_mutex_lock(&resources->stop_flag_lock);
			resources->stop_flag = 1;
			pthread_mutex_unlock(&resources->stop_flag_lock);
			pthread_mutex_lock(&resources->print_lock);
			printf("%ldms %d died\n",
				get_time_interval_ms(resources->initial_time, current_time), curr_philo_node->philosopher);
			pthread_mutex_unlock(&resources->print_lock);
			break ;
		}
		pthread_mutex_unlock(&curr_philo_node->pmits_lock);
		pthread_mutex_lock(&resources->full_philos_lock);
		if (resources->full_philos == resources->requested_philos)
		{
			pthread_mutex_lock(&resources->stop_flag_lock);
			resources->stop_flag = 1;
			pthread_mutex_unlock(&resources->stop_flag_lock);
			pthread_mutex_unlock(&resources->full_philos_lock);
			break ;
		}
		pthread_mutex_unlock(&resources->full_philos_lock);
		if (curr_philo_node->next != NULL)
			curr_philo_node = curr_philo_node->next;
		else
			curr_philo_node = resources->philosopher_list;
	}
	return (NULL);
}

void	*philosophers_routine(void *arg)
{
	t_resources			*resources;
	t_philosopher_list	*philosopher_node;
	
	resources = (t_resources*)arg;
	philosopher_node = resources->philosopher_list;
	pthread_mutex_lock(&resources->node_ready_flag_lock);
	philosopher_node = resources->curr_philo_node;
	resources->node_ready_flag = 1;
	pthread_mutex_unlock(&resources->node_ready_flag_lock);
	while (!pthread_mutex_lock(&resources->start_simulation_flag_lock) && resources->start_simulation_flag == 0)
	{
		pthread_mutex_unlock(&resources->start_simulation_flag_lock);
		usleep(1000);
	}
	if (resources->start_simulation_flag == 2)
	{
		pthread_mutex_unlock(&resources->start_simulation_flag_lock);
		return (NULL);
	}
	pthread_mutex_unlock(&resources->start_simulation_flag_lock);
	pthread_mutex_lock(&philosopher_node->pmits_lock);
	philosopher_node->prev_meal_or_initial_ts = resources->initial_time;
	pthread_mutex_unlock(&philosopher_node->pmits_lock);
	if (philosopher_node->philosopher % 2 == 1)
		odd_tasks(resources, philosopher_node);
	else
		even_tasks(resources, philosopher_node);
	return (NULL);
}
