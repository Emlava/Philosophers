/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:01:28 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/09 18:26:07 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	manage_philosopher_list(t_resources *resources)
{
	int					created_philos;
	t_philosopher_list	*curr_philo_node;
	int					return_value;
	pthread_t			monitor;
	
	created_philos = 0;
	resources->seat_nbr = 0;
	curr_philo_node = resources->philosopher_list;
	resources->error_creating_thread_flag = 0;
	resources->stop_flag = 0;
	return_value = 1;
	gettimeofday(&resources->initial_time, NULL);
	if (pthread_create(&monitor, NULL, monitor_routine, resources) != 0)
		return_value = 0;
	while (created_philos++ < resources->requested_philos && return_value == 1)
	{
		if (pthread_create(&curr_philo_node->thread, NULL, philosophers_routine, resources) != 0)
		{
			resources->error_creating_thread_flag = 1;
			pthread_detach(monitor);
			return_value = 0;
		}
		curr_philo_node = curr_philo_node->next;
	}






	// Here's the cleanup
	curr_philo_node = resources->philosopher_list;
	if (return_value == 1)
	{
		while (resources->requested_philos--)
		{
			pthread_join(curr_philo_node->thread, NULL);
			curr_philo_node = curr_philo_node->next;
		}
		pthread_join(monitor, NULL);
	}
	else
	{
		while (--created_philos)
		{
			pthread_detach(curr_philo_node->thread);
			curr_philo_node = curr_philo_node->next;
		}
	}
	free_philos_list(resources->philosopher_list);
	destroy_forks(resources->forks, resources->requested_philos);
	pthread_mutex_destroy(&resources->philo_nbr_lock);
	pthread_mutex_destroy(&resources->print_lock);
	return (return_value);
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
	if (!create_forks(&resources))
		return (3);
	if (!create_locks(&resources))
		return (4);
	if (!allocate_philos_list(&resources.philosopher_list, resources.requested_philos))
	{
		// Find a way to destroy the forks and locks inside the called function
		destroy_forks(resources.forks, resources.requested_philos);
		pthread_mutex_destroy(&resources.philo_nbr_lock);
		pthread_mutex_destroy(&resources.print_lock);
		return (5);
	}
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
