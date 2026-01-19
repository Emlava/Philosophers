/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   odd_tasks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 19:48:33 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/19 21:32:32 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_resources *resources,
	t_philosopher_list *philosopher_node, pthread_mutex_t **left_fork,
	pthread_mutex_t **right_fork)
{
	if (resources->requested_philos != 1)
	{
		if (philosopher_node->philosopher > 1)
			*left_fork = &resources->forks[philosopher_node->philosopher - 2];
		else
			*left_fork = &resources->forks[resources->requested_philos - 1];
	}
	*right_fork = &resources->forks[philosopher_node->philosopher - 1];
	return ;
}

static void	manage_one_philo(t_resources *resources,
	pthread_mutex_t *right_fork, t_philosopher_list *philosopher_node)
{
	pthread_mutex_lock(right_fork);
	print_state_change(resources, TF, philosopher_node);
	while (!pthread_mutex_lock(&resources->stop_flag_lock)
		&& resources->stop_flag != 1)
	{
		pthread_mutex_unlock(&resources->stop_flag_lock);
		usleep(2000);
	}
	pthread_mutex_unlock(&resources->stop_flag_lock);
	return ;
}

static int	wait_for_odds_eat_flag(t_resources *resources)
{
	while (!pthread_mutex_lock(&resources->odds_eat_flag_lock)
		&& !resources->odds_eat_flag)
	{
		pthread_mutex_unlock(&resources->odds_eat_flag_lock);
		usleep(1000);
		pthread_mutex_lock(&resources->stop_flag_lock);
		if (resources->stop_flag == 1)
		{
			pthread_mutex_unlock(&resources->stop_flag_lock);
			return (0);
		}
		pthread_mutex_unlock(&resources->stop_flag_lock);
	}
	pthread_mutex_unlock(&resources->odds_eat_flag_lock);
	return (1);
}

static void	keep_track_of_meals(int *total_meals_had, t_resources *resources)
{
	(*total_meals_had)++;
	pthread_mutex_lock(&resources->odds_meal_count_lock);
	resources->odds_meal_count++;
	pthread_mutex_unlock(&resources->odds_meal_count_lock);
	if (*total_meals_had == resources->nbr_of_meals)
	{
		pthread_mutex_lock(&resources->full_philos_lock);
		resources->full_philos++;
		pthread_mutex_unlock(&resources->full_philos_lock);
	}
}

void	odd_tasks(t_resources *resources, t_philosopher_list *philosopher_node)
{
	int				total_meals_had;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct timeval	curr_time;

	total_meals_had = 0;
	assign_forks(resources, philosopher_node, &left_fork, &right_fork);
	if (resources->requested_philos == 1)
	{
		manage_one_philo(resources, right_fork, philosopher_node);
		return ;
	}
	while (1)
	{
		if (!wait_for_odds_eat_flag(resources))
			return ;
		if (!start_eating(left_fork, right_fork, resources, philosopher_node))
			return ;
		keep_track_of_meals(&total_meals_had, resources);
		wait_time_to_eat(&curr_time, philosopher_node, resources);
		pthread_mutex_unlock(left_fork);
		pthread_mutex_unlock(right_fork);
		if (!sleep_and_think(resources, philosopher_node))
			return ;
	}
}
