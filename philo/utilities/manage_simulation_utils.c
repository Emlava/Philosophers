/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_simulation_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 14:52:02 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/18 14:53:12 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	assign_resources(t_resources *resources, int *created_philos,
	int *return_value)
{
	*created_philos = 0;
	resources->curr_philo_node = resources->philosopher_list;
	if (resources->requested_philos % 2 == 1)
		resources->odd_philos = resources->requested_philos / 2 + 1;
	else
		resources->odd_philos = resources->requested_philos / 2;
	resources->even_philos = resources->requested_philos / 2;
	resources->odds_meal_count = 0;
	resources->evens_meal_count = 0;
	resources->error_creating_thread_flag = 0;
	resources->node_ready_flag = 0;
	resources->start_simulation_flag = 0;
	resources->odds_eat_flag = 0;
	resources->evens_eat_flag = 0;
	resources->full_philos = 0;
	resources->stop_flag = 0;
	*return_value = 1;
	return ;
}

void	wait_for_node_to_be_ready(t_resources *resources)
{
	while (!pthread_mutex_lock(&resources->node_ready_flag_lock)
		&& resources->node_ready_flag == 0)
	{
		pthread_mutex_unlock(&resources->node_ready_flag_lock);
		usleep(1000);
	}
	resources->node_ready_flag = 0;
	resources->curr_philo_node = resources->curr_philo_node->next;
	pthread_mutex_unlock(&resources->node_ready_flag_lock);
	return ;
}
