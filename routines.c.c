/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:29:34 by elara-va          #+#    #+#             */
/*   Updated: 2025/12/21 20:15:45 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_resources	*resources;
	
	resources = (t_resources*)arg;
	while (resources->all_meals_or_death_flag == 0)
		;
	resources->stop_flag = 1;
	return (NULL);
}

void	*philosophers_routine(void *arg)
{
	t_resources		*resources;
	int				philosopher;
	
	resources = (t_resources*)arg;
	pthread_mutex_lock(&resources->philo_nbr_lock);
	philosopher = ++resources->philo_nbr;
	pthread_mutex_unlock(&resources->philo_nbr_lock);
	if (resources->nbr_of_meals == -1)
	{
		// Function that runs the loop and has a check of stop_flag before every printing of state
		// If the flag is 1, the thread returns;
		// Eating
		// Thinking
		// Sleeping
	}
	//
	print_state_change(resources->initial_time, philosopher, D, &resources->print_lock);
	//
	
	return (NULL);
}
