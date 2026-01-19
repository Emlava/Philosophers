/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:29:34 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/19 19:37:31 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosophers_routine(void *arg)
{
	t_resources			*resources;
	t_philosopher_list	*philosopher_node;

	resources = (t_resources *)arg;
	philosopher_node = resources->philosopher_list;
	pthread_mutex_lock(&resources->node_ready_flag_lock);
	philosopher_node = resources->curr_philo_node;
	resources->node_ready_flag = 1;
	pthread_mutex_unlock(&resources->node_ready_flag_lock);
	if (!wait_for_simulation_to_start(resources))
		return (NULL);
	pthread_mutex_lock(&philosopher_node->pmits_lock);
	philosopher_node->prev_meal_or_initial_ts = resources->initial_time;
	pthread_mutex_unlock(&philosopher_node->pmits_lock);
	if (philosopher_node->philosopher % 2 == 1)
		odd_tasks(resources, philosopher_node);
	else
		even_tasks(resources, philosopher_node);
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_resources			*resources;
	t_philosopher_list	*curr_philo_node;
	struct timeval		current_time;

	resources = (t_resources *)arg;
	curr_philo_node = resources->philosopher_list;
	while (1)
	{
		if (!wait_for_philo_to_be_ready(resources, curr_philo_node))
			return (NULL);
		gettimeofday(&current_time, NULL);
		if (!check_for_pulse(curr_philo_node, current_time, resources))
			return (NULL);
		if (!check_if_all_philos_are_full(resources))
			return (NULL);
		if (curr_philo_node->next != NULL)
			curr_philo_node = curr_philo_node->next;
		else
			curr_philo_node = resources->philosopher_list;
	}
}
