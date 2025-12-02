/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 20:55:38 by emlava            #+#    #+#             */
/*   Updated: 2025/12/02 21:15:05 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// The mutexes must be unlocked
void	destroy_mutexes(pthread_mutex_t *forks, int nbr_of_forks)
{
	int	i;

	i = 0;
	while (i < nbr_of_forks)
		pthread_mutex_destroy(&(forks[i++]));
	free(forks);
	return ;
}

int	manage_forks(pthread_mutex_t **forks, int nbr_of_forks)
{
	int	i;

	*forks = malloc(sizeof(pthread_mutex_t) * nbr_of_forks);
	if (!*forks)
		return (0);
	i = 0;
	while (i < nbr_of_forks)
	{
		if (pthread_mutex_init(&(*forks)[i], NULL) != 0)
		{
			destroy_mutexes(*forks, i);
			return (0);
		}
		i++;
	}
	return (1);
}

void	convert_args_to_int(char *av[], t_resources *resources, int ac)
{
	resources->nbr_of_philos = ft_atoi(av[1]);
	resources->time_to_die = ft_atoi(av[2]);
	resources->time_to_eat = ft_atoi(av[3]);
	resources->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		resources->nbr_of_meals = ft_atoi(av[5]);
	else
		resources->nbr_of_meals = -1;
	return ;
}

int main(int ac, char *av[])
{
	t_resources	resources;
	t_threads	*curr_thread;
	
	if (ac < 5 || ac > 6)
	{
		write(2, "Incorrect number of arguments\n", 30);
		return (1);
	}
	convert_args_to_int(av, &resources, ac);
	if (!manage_forks(&resources.forks, resources.nbr_of_philos));
		return (1);
	// Allocate each node of t_threads (as many as there are philosophers)
	// Create threads/philosophers on a while loop
	while (ac--)
	{
		pthread_create();
		curr_thread = curr_thread->next;
	}
	return (0);
}

// Things to free/destroy:
// -forks pointer
// -Each mutex
