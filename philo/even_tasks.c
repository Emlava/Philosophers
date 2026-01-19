/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   even_tasks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 15:18:35 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/19 21:35:53 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(pthread_mutex_t **left_fork,
	pthread_mutex_t **right_fork,	t_resources *resources,
	t_philosopher_list *philosopher_node)
{
	*left_fork = &resources->forks[philosopher_node->philosopher - 2];
	*right_fork = &resources->forks[philosopher_node->philosopher - 1];
	return ;
}

static int	wait_for_evens_eat_flag(t_resources *resources)
{
	while (!pthread_mutex_lock(&resources->evens_eat_flag_lock)
		&& !resources->evens_eat_flag)
	{
		pthread_mutex_unlock(&resources->evens_eat_flag_lock);
		usleep(1000);
		pthread_mutex_lock(&resources->stop_flag_lock);
		if (resources->stop_flag == 1)
		{
			pthread_mutex_unlock(&resources->stop_flag_lock);
			return (0);
		}
		pthread_mutex_unlock(&resources->stop_flag_lock);
	}
	pthread_mutex_unlock(&resources->evens_eat_flag_lock);
	return (1);
}

static void	keep_track_of_meals(int *total_meals_had, t_resources *resources)
{
	(*total_meals_had)++;
	pthread_mutex_lock(&resources->evens_meal_count_lock);
	resources->evens_meal_count++;
	pthread_mutex_unlock(&resources->evens_meal_count_lock);
	if (*total_meals_had == resources->nbr_of_meals)
	{
		pthread_mutex_lock(&resources->full_philos_lock);
		resources->full_philos++;
		pthread_mutex_unlock(&resources->full_philos_lock);
	}
	return ;
}

void	even_tasks(t_resources *resources, t_philosopher_list *philosopher_node)
{
	int				total_meals_had;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct timeval	curr_time;

	total_meals_had = 0;
	assign_forks(&left_fork, &right_fork, resources, philosopher_node);
	while (1)
	{
		if (!sleep_and_think(resources, philosopher_node))
			return ;
		if (!wait_for_evens_eat_flag(resources))
			return ;
		if (!start_eating(left_fork, right_fork, resources, philosopher_node))
			return ;
		keep_track_of_meals(&total_meals_had, resources);
		wait_time_to_eat(&curr_time, philosopher_node, resources);
		pthread_mutex_unlock(left_fork);
		pthread_mutex_unlock(right_fork);
	}
}
