/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 20:55:38 by emlava            #+#    #+#             */
/*   Updated: 2025/12/07 15:56:50 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	launch_threads(t_resources resources, t_threads *threads)
{
	t_threads	*curr_thread;
	
	curr_thread = threads;
	// Create threads/philosophers on a while loop
	while (resources.nbr_of_philos--)
	{
		// pthread_create();
		curr_thread = curr_thread->next;
	}
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
		destroy_forks(&resources.forks, resources.nbr_of_philos);
		return (4);
	}
	if (!launch_threads(resources, threads))
		return (5);
	return (0);
}

// Things to free/destroy:
// -Each mutex (fork)
// -forks pointer
// -thread list
