/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 20:55:38 by emlava            #+#    #+#             */
/*   Updated: 2025/12/15 20:54:15 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// This function is only run by the philosophers
void	*start_routine(void *arg)
{
	t_resources		*resources;
	int				philosopher;
	
	resources = (t_resources*)arg;
	pthread_mutex_lock(&resources->philo_nbr_lock);
	philosopher = ++resources->philo_nbr;
	pthread_mutex_unlock(&resources->philo_nbr_lock);
	//
	printf("Philosopher %d exists\n", philosopher);
	//
	// Eating
	// Thinking
	// Sleeping
	
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
		if (pthread_create(&curr_philosopher->thread, NULL, start_routine, resources) != 0)
		{
			return_value = 0;
			break ;
		}
		curr_philosopher = curr_philosopher->next;
	}
	curr_philosopher = philosophers;
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
	pthread_mutex_destroy(&resources->philo_nbr_lock);
	pthread_mutex_destroy(&resources->print_lock);
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
	if (!create_forks(&resources))
		return (3);
	if (!create_locks(&resources))
		return (4);
	if (!allocate_philos_list(&philosophers, resources.requested_philos))
	{
		// Find a way to destroy the forks and locks inside the called function
		destroy_forks(resources.forks, resources.requested_philos);
		pthread_mutex_destroy(&resources.philo_nbr_lock);
		pthread_mutex_destroy(&resources.print_lock);
		return (5);
	}
	if (!manage_philosophers(&resources, philosophers))
	{
		write(2, "Failed to create thread\n", 24);
		return (6);
	}
	return (0);
}

// Things to free/destroy:
// -Each mutex (forks and locks)
// -forks pointer
// -Each thread (detach/join)
// -thread list (free)
