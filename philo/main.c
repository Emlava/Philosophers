/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:01:28 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/18 13:48:45 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_resources(t_resources *resources, int *created_philos, int *return_value)
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

static void	spawn_philos_and_monitor(t_resources *resources, int *created_philos, int *return_value, pthread_t *monitor)
{
	while (*created_philos < resources->requested_philos && *return_value == 1)
	{
		if (pthread_create(&resources->curr_philo_node->thread, NULL, philosophers_routine, resources) != 0)
		{
			pthread_mutex_lock(&resources->ect_flag_lock);
			resources->error_creating_thread_flag = 1;
			pthread_mutex_unlock(&resources->ect_flag_lock);
			*return_value = 0;
		}
		else
		{
			while (!pthread_mutex_lock(&resources->node_ready_flag_lock) && resources->node_ready_flag == 0)
			{
				pthread_mutex_unlock(&resources->node_ready_flag_lock);
				usleep(1000);
			}
			resources->node_ready_flag = 0;
			resources->curr_philo_node = resources->curr_philo_node->next;
			pthread_mutex_unlock(&resources->node_ready_flag_lock);
			(*created_philos)++;
		}
	}
	if (*return_value == 1 && pthread_create(monitor, NULL, monitor_routine, resources) != 0)
		*return_value = 0;
	return ;
}

static int	kick_off_simulation(t_resources *resources, int created_philos, int return_value, pthread_t monitor)
{
	gettimeofday(&resources->initial_time, NULL);
	if (return_value == 1)
	{
		pthread_mutex_lock(&resources->start_simulation_flag_lock);
		resources->start_simulation_flag = 1;
		pthread_mutex_unlock(&resources->start_simulation_flag_lock);
		return (1);
	}
	else
	{
		pthread_mutex_lock(&resources->start_simulation_flag_lock);
		resources->start_simulation_flag = 2;
		pthread_mutex_unlock(&resources->start_simulation_flag_lock);
		full_cleanup(resources, return_value, created_philos, monitor);
		return (0);
	}
}

static int	odds_eat(t_resources *resources, int created_philos, int return_value, pthread_t monitor)
{
	pthread_mutex_lock(&resources->odds_eat_flag_lock);
	resources->odds_eat_flag = 1;
	pthread_mutex_unlock(&resources->odds_eat_flag_lock);
	while (!pthread_mutex_lock(&resources->odds_meal_count_lock) && resources->odds_meal_count != resources->odd_philos)
	{
		pthread_mutex_unlock(&resources->odds_meal_count_lock);
		usleep(2000);
		pthread_mutex_lock(&resources->stop_flag_lock);
		if (resources->stop_flag == 1)
		{
			pthread_mutex_unlock(&resources->stop_flag_lock);
			full_cleanup(resources, return_value, created_philos, monitor);
			return (0);
		}
		pthread_mutex_unlock(&resources->stop_flag_lock);
	}
	resources->odds_eat_flag = 0; // This doesn't need a lock because when we exit the previous while loop
	// the threads will be doing the other tasks or waiting for the meal count to restart (below)
	resources->odds_meal_count = 0;
	pthread_mutex_unlock(&resources->odds_meal_count_lock);
	return (1);
}

static int	evens_eat(t_resources *resources, int created_philos, int return_value, pthread_t monitor)
{
	pthread_mutex_lock(&resources->evens_eat_flag_lock);
	resources->evens_eat_flag = 1;
	pthread_mutex_unlock(&resources->evens_eat_flag_lock);
	while (!pthread_mutex_lock(&resources->evens_meal_count_lock) && resources->evens_meal_count != resources->even_philos)
	{
		pthread_mutex_unlock(&resources->evens_meal_count_lock);
		usleep(2000);
		pthread_mutex_lock(&resources->stop_flag_lock);
		if (resources->stop_flag == 1)
		{
			pthread_mutex_unlock(&resources->stop_flag_lock);
			full_cleanup(resources, return_value, created_philos, monitor);
			return (0);
		}
		pthread_mutex_unlock(&resources->stop_flag_lock);
	}
	resources->evens_eat_flag = 0; // This doesn't need a lock because when we exit the previous while loop
	// the threads will be doing the other tasks or waiting for the meal count to restart (below)
	resources->evens_meal_count = 0;
	pthread_mutex_unlock(&resources->evens_meal_count_lock);
	return (1);
}

static int	manage_philosopher_list(t_resources *resources)
{
	int					created_philos;
	int					return_value;
	pthread_t			monitor;
	
	assign_resources(resources, &created_philos, &return_value);
	spawn_philos_and_monitor(resources, &created_philos, &return_value, &monitor);
	if (!kick_off_simulation(resources, created_philos, return_value, monitor))
		return (return_value);
	while (1)
	{
		if (!odds_eat(resources, created_philos, return_value, monitor))
			return (return_value);
		if (!evens_eat(resources, created_philos, return_value, monitor))
			return (return_value);
	}
}

int main(int ac, char *av[])
{
	t_resources		resources;
	
	if (ac < 5 || ac > 6)
	{
		write(2, "Incorrect number of arguments\n", 30);
		return (1);
	}
	if (!convert_args_to_int(av, &resources, ac))
		return (2);
	if (resources.nbr_of_meals == 0)
		return (0);
	if (!allocate_philos_list(&resources.philosopher_list, resources.requested_philos))
		return (3);
	if (!create_forks(&resources))
		return (4);
	if (!create_locks(&resources))
		return (5);
	if (!manage_philosopher_list(&resources))
	{
		write(2, "Failed to create thread\n", 24);
		return (6);
	}
	return (0);
}

// Things to free/destroy and threads to join/detach:
// -Each mutex (forks and locks)
// -forks pointer
// -Each thread (detach/join)
// -thread list (free)
// Join/detach philosopher_list
// Join/detach monitor
