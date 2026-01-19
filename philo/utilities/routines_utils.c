/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 19:40:37 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/19 19:41:58 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	wait_for_simulation_to_start(t_resources *resources)
{
	while (!pthread_mutex_lock(&resources->start_simulation_flag_lock)
		&& resources->start_simulation_flag == 0)
	{
		pthread_mutex_unlock(&resources->start_simulation_flag_lock);
		usleep(1000);
	}
	if (resources->start_simulation_flag == 2)
	{
		pthread_mutex_unlock(&resources->start_simulation_flag_lock);
		return (0);
	}
	pthread_mutex_unlock(&resources->start_simulation_flag_lock);
	return (1);
}

int	wait_for_philo_to_be_ready(t_resources *resources,
	t_philosopher_list *philo_node)
{
	while (!pthread_mutex_lock(&philo_node->pmits_lock)
		&& philo_node->prev_meal_or_initial_ts.tv_sec == -1)
	{
		pthread_mutex_unlock(&philo_node->pmits_lock);
		pthread_mutex_lock(&resources->ect_flag_lock);
		if (resources->error_creating_thread_flag)
		{
			pthread_mutex_unlock(&resources->ect_flag_lock);
			pthread_mutex_lock(&resources->stop_flag_lock);
			resources->stop_flag = 1;
			pthread_mutex_unlock(&resources->stop_flag_lock);
			return (0);
		}
		pthread_mutex_unlock(&resources->ect_flag_lock);
	}
	// We don't unlock pmits_lock here because it's used in the next
	// if statement in routines.c
	return (1);
}

int	check_for_pulse(t_philosopher_list *philo_node,
	struct timeval current_time, t_resources *resources)
{
	if (get_time_interval_ms(philo_node->prev_meal_or_initial_ts,
			current_time) >= resources->time_to_die)
	{
		pthread_mutex_unlock(&philo_node->pmits_lock);
		pthread_mutex_lock(&resources->stop_flag_lock);
		resources->stop_flag = 1;
		pthread_mutex_unlock(&resources->stop_flag_lock);
		pthread_mutex_lock(&resources->print_lock);
		printf("%ldms %d died\n",
			get_time_interval_ms(resources->initial_time, current_time),
			philo_node->philosopher);
		pthread_mutex_unlock(&resources->print_lock);
		return (0);
	}
	pthread_mutex_unlock(&philo_node->pmits_lock);
	return (1);
}

int	check_if_all_philos_are_full(t_resources *resources)
{
	pthread_mutex_lock(&resources->full_philos_lock);
	if (resources->full_philos == resources->requested_philos)
	{
		pthread_mutex_lock(&resources->stop_flag_lock);
		resources->stop_flag = 1;
		pthread_mutex_unlock(&resources->stop_flag_lock);
		pthread_mutex_unlock(&resources->full_philos_lock);
		return (0);
	}
	pthread_mutex_unlock(&resources->full_philos_lock);
	return (1);
}
