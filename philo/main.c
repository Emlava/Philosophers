/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 20:55:38 by emlava            #+#    #+#             */
/*   Updated: 2025/12/06 19:15:19 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
			destroy_forks(*forks, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	convert_args_to_int(char *av[], t_resources *resources, int ac)
{
	resources->nbr_of_philos = ft_atoi(av[1]);
	if (resources->nbr_of_philos <= 0)
		return (0);
	resources->time_to_die = ft_atoi(av[2]);
	resources->time_to_eat = ft_atoi(av[3]);
	resources->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		resources->nbr_of_meals = ft_atoi(av[5]);
	else
		resources->nbr_of_meals = -1;
	if (resources->nbr_of_philos <= 0 || resources->time_to_die < 0
		|| resources->time_to_eat < 0 || resources->time_to_sleep < 0
		|| (ac == 6 && resources->nbr_of_meals <= 0))
		return (0);
	else
		return (1);
}

int	allocate_thread_list(t_threads **threads, t_threads **curr_thread, int nbr_of_philos)
{
	int	i;

	*threads = malloc(sizeof(t_threads));
	if (!(*threads))
		return (0);
	(*threads)->next = NULL;
	*curr_thread = *threads;
	i = 1;
	while (i++ < nbr_of_philos)
	{
		(*curr_thread)->next = malloc(sizeof(t_threads));
		if (!(*curr_thread)->next)
		{
			(*curr_thread)->next = NULL;
			free_thread_list(*threads);	
			return (0);
		}
		*curr_thread = (*curr_thread)->next;
	}
	(*curr_thread)->next = NULL;
	return (1);
}

int main(int ac, char *av[])
{
	t_resources	resources;
	t_threads	*threads;
	t_threads	*curr_thread;
	
	if (ac < 5 || ac > 6)
	{
		write(2, "Incorrect number of arguments\n", 30);
		return (1);
	}
	if (!convert_args_to_int(av, &resources, ac))
		return (2);
	if (!manage_forks(&resources.forks, resources.nbr_of_philos));
		return (3);
	if (!allocate_thread_list(&threads, &curr_thread, resources.nbr_of_philos))
		// destroy forks and free its pointer
		return (4);
	// Create threads/philosophers on a while loop
	while (resources.nbr_of_philos--)
	{
		// pthread_create();
		curr_thread = curr_thread->next;
	}
	return (0);
}

// Things to free/destroy:
// -Each mutex (fork)
// -forks pointer
// -thread list
