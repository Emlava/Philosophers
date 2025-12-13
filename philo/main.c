/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 20:55:38 by emlava            #+#    #+#             */
/*   Updated: 2025/12/13 19:58:27 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// This function is only run by the philosophers
void	*start_routine(void *arg)
{
	t_resources		*resources;
	pthread_mutex_t	nbr_lock;
	int				philosopher;
	
	resources = (t_resources*)arg;
	// Eating
	// Thinking
	// Sleeping
	
	// START HERE TOMORROW
	// Lock philo_nbr with nbr_lock and assign it to philosopher
	
	//
	printf("Philosopher %d exists\n", resources->philo_nbr);
	//
	return (NULL);
}

int	manage_philosophers(t_resources *resources, t_philosophers *philosophers)
{
	int				created_philos;
	t_philosophers	*curr_philosopher;
	int				return_value;
	
	created_philos = 0;
	resources->philo_nbr = 0;
	curr_philosopher = philosophers;
	return_value = 1;
	while (created_philos++ < resources->requested_philos)
	{
		if (pthread_create(curr_philosopher->thread, NULL, start_routine, resources) != 0)
		{
			return_value = 0;
			break ;
		}
		curr_philosopher = curr_philosopher->next;
	}
	curr_philosopher = philosophers;
	// if (return_value == 1) Join the created philosophers? (Find way to check for deaths)
	if (return_value == 1)
	{
		while (resources->requested_philos--)
		{
			pthread_join(curr_philosopher->thread, NULL);
			curr_philosopher = curr_philosopher->next;
		}
	}
	else
	{
		while (--created_philos)
		{
			pthread_detach(curr_philosopher->thread);
			curr_philosopher = curr_philosopher->next;
		}
	}
	free_philos_list(philosophers);
	destroy_forks(resources->forks, resources->requested_philos);
	return (return_value);
}

int main(int ac, char *av[])
{
	t_resources	resources;
	t_philosophers	*philosophers;
	
	if (ac < 5 || ac > 6)
	{
		write(2, "Incorrect number of arguments\n", 30);
		return (1);
	}
	if (!convert_args_to_int(av, &resources, ac))
		return (2);
	if (!create_forks(&resources.forks, resources.requested_philos))
		return (3);
	if (!allocate_philos_list(&philosophers, resources.requested_philos))
	{
		// Find a way to destroy the forks inside the called function
		destroy_forks(resources.forks, resources.requested_philos);
		return (4);
	}
	if (!manage_philosophers(&resources, philosophers))
	{
		write(2, "Failed to create thread\n", 24);
		return (5);
	}
	return (0);
}

// Things to free/destroy:
// -Each mutex (fork)
// -forks pointer
// -Each thread (detach/join)
// -thread list (free)
