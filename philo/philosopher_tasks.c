/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_tasks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 15:18:35 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/18 15:22:19 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	odd_tasks(t_resources *resources, t_philosopher_list *philosopher_node)
{
	int				total_meals_had;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct timeval	curr_time;
	
	total_meals_had = 0;
	if (resources->requested_philos != 1)
	{
		if (philosopher_node->philosopher > 1)
			left_fork = &resources->forks[philosopher_node->philosopher - 2];
		else
			left_fork = &resources->forks[resources->requested_philos - 1];
	}
	right_fork = &resources->forks[philosopher_node->philosopher - 1];
	if (resources->requested_philos == 1)
	{
		pthread_mutex_lock(right_fork);
		print_state_change(resources, TF, philosopher_node);
		while (!pthread_mutex_lock(&resources->stop_flag_lock) && resources->stop_flag != 1)
		{
			pthread_mutex_unlock(&resources->stop_flag_lock);
			usleep(2000);
		}
		pthread_mutex_unlock(&resources->stop_flag_lock);
		return ;
	}
	while (1)
	{
		while (!pthread_mutex_lock(&resources->odds_eat_flag_lock) && !resources->odds_eat_flag)
		{
			pthread_mutex_unlock(&resources->odds_eat_flag_lock);
			usleep(1000);
			pthread_mutex_lock(&resources->stop_flag_lock);
			if (resources->stop_flag == 1)
			{
				pthread_mutex_unlock(&resources->stop_flag_lock);
				return ;
			}
			pthread_mutex_unlock(&resources->stop_flag_lock);
		}
		pthread_mutex_unlock(&resources->odds_eat_flag_lock);
		pthread_mutex_lock(left_fork);
		if (!print_state_change(resources, TF, philosopher_node))
		{
			pthread_mutex_unlock(left_fork);
			break ;
		}
		pthread_mutex_lock(right_fork);
		if (!print_state_change(resources, TF, philosopher_node))
		{
			pthread_mutex_unlock(left_fork);
			pthread_mutex_unlock(right_fork);
			break ;
		}
		if (!print_state_change(resources, E, philosopher_node))
		{
			pthread_mutex_unlock(left_fork);
			pthread_mutex_unlock(right_fork);
			break ;
		}
		total_meals_had++;
		pthread_mutex_lock(&resources->odds_meal_count_lock);
		resources->odds_meal_count++;
		pthread_mutex_unlock(&resources->odds_meal_count_lock);
		if (total_meals_had == resources->nbr_of_meals)
		{
			pthread_mutex_lock(&resources->full_philos_lock);
			resources->full_philos++;
			pthread_mutex_unlock(&resources->full_philos_lock);
		}
		curr_time = philosopher_node->prev_meal_or_initial_ts;
		while (!pthread_mutex_lock(&philosopher_node->pmits_lock)
			&& get_time_interval_ms(philosopher_node->prev_meal_or_initial_ts, curr_time) < resources->time_to_eat)
		{
			pthread_mutex_unlock(&philosopher_node->pmits_lock);
			gettimeofday(&curr_time, NULL);
		}
		pthread_mutex_unlock(&philosopher_node->pmits_lock);
		pthread_mutex_unlock(left_fork);
		pthread_mutex_unlock(right_fork);
		if (!print_state_change(resources, S, philosopher_node))
			break ;
		usleep(resources->time_to_sleep * 1000);
		if (!print_state_change(resources, T, philosopher_node))
			break ;		
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
	left_fork = &resources->forks[philosopher_node->philosopher - 2];
	right_fork = &resources->forks[philosopher_node->philosopher - 1];

	while (1)
	{	
		if (!print_state_change(resources, S, philosopher_node))
			break ;
		usleep(resources->time_to_sleep * 1000);
		if (!print_state_change(resources, T, philosopher_node))
			break ;
		while (!pthread_mutex_lock(&resources->evens_eat_flag_lock) && !resources->evens_eat_flag)
		{
			pthread_mutex_unlock(&resources->evens_eat_flag_lock);
			usleep(1000);
			pthread_mutex_lock(&resources->stop_flag_lock);
			if (resources->stop_flag == 1)
			{
				pthread_mutex_unlock(&resources->stop_flag_lock);
				return ;
			}
			pthread_mutex_unlock(&resources->stop_flag_lock);
		}
		pthread_mutex_unlock(&resources->evens_eat_flag_lock);
		pthread_mutex_lock(left_fork);
		if (!print_state_change(resources, TF, philosopher_node))
		{
			pthread_mutex_unlock(left_fork);
			break ;
		}
		pthread_mutex_lock(right_fork);
		if (!print_state_change(resources, TF, philosopher_node))
		{
			pthread_mutex_unlock(left_fork);
			pthread_mutex_unlock(right_fork);
			break ;
		}
		if (!print_state_change(resources, E, philosopher_node))
		{
			pthread_mutex_unlock(left_fork);
			pthread_mutex_unlock(right_fork);
			break ;
		}
		total_meals_had++;
		pthread_mutex_lock(&resources->evens_meal_count_lock);
		resources->evens_meal_count++;
		pthread_mutex_unlock(&resources->evens_meal_count_lock);
		if (total_meals_had == resources->nbr_of_meals)
		{
			pthread_mutex_lock(&resources->full_philos_lock);
			resources->full_philos++;
			pthread_mutex_unlock(&resources->full_philos_lock);
		}
		curr_time = philosopher_node->prev_meal_or_initial_ts;
		while (!pthread_mutex_lock(&philosopher_node->pmits_lock)
			&& get_time_interval_ms(philosopher_node->prev_meal_or_initial_ts, curr_time) < resources->time_to_eat)
		{
			pthread_mutex_unlock(&philosopher_node->pmits_lock);
			gettimeofday(&curr_time, NULL);
		}
		pthread_mutex_unlock(&philosopher_node->pmits_lock);
		pthread_mutex_unlock(left_fork);
		pthread_mutex_unlock(right_fork);		
	}
	return ;
}
