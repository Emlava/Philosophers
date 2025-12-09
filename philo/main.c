/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 20:55:38 by emlava            #+#    #+#             */
/*   Updated: 2025/12/09 21:18:24 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*start_routine(void *resources)
{}

int	launch_threads(t_resources *resources, t_threads *threads)
{
	t_threads	*curr_thread;
	
	resources->curr_philo = 1;
	curr_thread = threads;
	// Create threads/philosophers on a while loop
	while (resources->curr_philo <= resources->nbr_of_philos)
	{
		if (pthread_create(curr_thread, NULL, start_routine, resources) != 0)
		{
			// Detach/join the created threads
			free_thread_list(threads);
			destroy_forks(resources->forks, resources->nbr_of_philos);
			return (0);
		}
		// This stores the ID of the new thread in the buffer pointed to by curr_thread
		curr_thread = curr_thread->next;
		resources->curr_philo++;
	}
	// Join or detach threads
}

int main(int ac, char *av[])
{
	t_resources	resources;
	t_threads	*threads;
	
	if (ac < 5 || ac > 6)
	{
		write(2, "Incorrect number of arguments\n", 30);
		return (1);
	}
	if (!convert_args_to_int(av, &resources, ac))
		return (2);
	if (!create_forks(&resources.forks, resources.nbr_of_philos));
		return (3);
	if (!allocate_thread_list(&threads, resources.nbr_of_philos))
	{
		// Find a way to destroy the forks inside the called function
		destroy_forks(&resources.forks, resources.nbr_of_philos);
		return (4);
	}
	if (!launch_threads(&resources, threads))
		return (5);
	return (0);
}

// Things to free/destroy:
// -Each mutex (fork)
// -forks pointer
// -Each thread (detach/join)
// -thread list (free)
