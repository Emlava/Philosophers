/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 20:30:23 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/19 21:33:53 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	start_eating(pthread_mutex_t *left_fork, pthread_mutex_t *right_fork,
	t_resources *resources, t_philosopher_list *philosopher_node)
{
	pthread_mutex_lock(left_fork);
	if (!print_state_change(resources, TF, philosopher_node))
	{
		pthread_mutex_unlock(left_fork);
		return (0);
	}
	pthread_mutex_lock(right_fork);
	if (!print_state_change(resources, TF, philosopher_node))
	{
		pthread_mutex_unlock(left_fork);
		pthread_mutex_unlock(right_fork);
		return (0);
	}
	if (!print_state_change(resources, E, philosopher_node))
	{
		pthread_mutex_unlock(left_fork);
		pthread_mutex_unlock(right_fork);
		return (0);
	}
	return (1);
}

void	wait_time_to_eat(struct timeval *curr_time,
	t_philosopher_list *philosopher_node, t_resources *resources)
{
	*curr_time = philosopher_node->prev_meal_or_initial_ts;
	while (!pthread_mutex_lock(&philosopher_node->pmits_lock)
		&& get_time_interval_ms(philosopher_node->prev_meal_or_initial_ts,
			*curr_time) < resources->time_to_eat)
	{
		pthread_mutex_unlock(&philosopher_node->pmits_lock);
		gettimeofday(curr_time, NULL);
	}
	pthread_mutex_unlock(&philosopher_node->pmits_lock);
	return ;
}

int	sleep_and_think(t_resources *resources,
	t_philosopher_list *philosopher_node)
{
	if (!print_state_change(resources, S, philosopher_node))
		return (0);
	usleep(resources->time_to_sleep * 1000);
	if (!print_state_change(resources, T, philosopher_node))
		return (0);
	return (1);
}
