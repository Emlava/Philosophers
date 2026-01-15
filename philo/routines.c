/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:29:34 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/15 15:28:33 by elara-va         ###   ########.fr       */
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

static void	evens_tasks(t_resources *resources, t_philosopher_list *philosopher_node)
{
	int				total_meals_had;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct timeval	curr_time;
	
	total_meals_had = 0;
	// Forks assignment
	left_fork = &resources->forks[philosopher_node->philosopher = 2];
	right_fork = &resources->forks[philosopher_node->philosopher - 1];

	while (1)
	{	
		// Sleeping
		if (!print_state_change(resources, S, philosopher_node))
			break ;
		usleep(resources->time_to_sleep * 1000);

		// Thinking
		if (!print_state_change(resources, T, philosopher_node))
			break ;

		// Eating
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
		gettimeofday(&curr_time, NULL);
		while (!pthread_mutex_lock(&philosopher_node->pmits_lock)
			&& get_time_interval_ms(philosopher_node->prev_meal_or_initial_ts, curr_time) < resources->time_to_eat)
		{
			pthread_mutex_unlock(&philosopher_node->pmits_lock);
			gettimeofday(&curr_time, NULL);
		}
		pthread_mutex_unlock(&philosopher_node->pmits_lock);
		pthread_mutex_unlock(left_fork);
		pthread_mutex_unlock(right_fork);
			
		// Waiting for odds_meal_count to go back to 0
		while (!pthread_mutex_lock(&resources->evens_meal_count_lock) && resources->evens_meal_count != 0)
		{
			pthread_mutex_unlock(&resources->evens_meal_count_lock);
			usleep(1000);
			pthread_mutex_lock(&resources->stop_flag_lock);
			if (resources->stop_flag == 1)
			{
				pthread_mutex_unlock(&resources->stop_flag_lock);
				return ;
			}
			pthread_mutex_unlock(&resources->stop_flag_lock);
		}
		pthread_mutex_unlock(&resources->evens_meal_count_lock);
	}
	return ;
}

static void	odds_tasks(t_resources *resources, t_philosopher_list *philosopher_node)
{
	int				total_meals_had;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct timeval	curr_time;
	
	total_meals_had = 0;

	// Forks assignment
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
		// Eating
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
		gettimeofday(&curr_time, NULL);
		while (!pthread_mutex_lock(&philosopher_node->pmits_lock)
			&& get_time_interval_ms(philosopher_node->prev_meal_or_initial_ts, curr_time) < resources->time_to_eat)
		{
			pthread_mutex_unlock(&philosopher_node->pmits_lock);
			gettimeofday(&curr_time, NULL);
		}
		pthread_mutex_unlock(&philosopher_node->pmits_lock);
		pthread_mutex_unlock(left_fork);
		pthread_mutex_unlock(right_fork);
		
		// Sleeping
		if (!print_state_change(resources, S, philosopher_node))
			break ;
		usleep(resources->time_to_sleep * 1000);

		// Thinking
		if (!print_state_change(resources, T, philosopher_node))
			break ;
			
		// Waiting for odds_meal_count to go back to 0
		while (!pthread_mutex_lock(&resources->odds_meal_count_lock) && resources->odds_meal_count != 0)
		{
			pthread_mutex_unlock(&resources->odds_meal_count_lock);
			usleep(1000);
			pthread_mutex_lock(&resources->stop_flag_lock);
			if (resources->stop_flag == 1)
			{
				pthread_mutex_unlock(&resources->stop_flag_lock);
				return ;
			}
			pthread_mutex_unlock(&resources->stop_flag_lock);
		}
		pthread_mutex_unlock(&resources->odds_meal_count_lock);
	}
	return ;
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

	// We wait (sleeping) here until main thread raises start_simulation_flag
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
	// The monitor is waiting for this
	pthread_mutex_lock(&philosopher_node->pmits_lock);
	philosopher_node->prev_meal_or_initial_ts = resources->initial_time;
	pthread_mutex_unlock(&philosopher_node->pmits_lock);
	//
	if (philosopher_node->philosopher % 2 == 1)
		odds_tasks(resources, philosopher_node);
	else
		evens_tasks(resources, philosopher_node);
	return (NULL);
}
