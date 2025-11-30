/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 20:55:38 by emlava            #+#    #+#             */
/*   Updated: 2025/11/30 17:42:06 by elara-va         ###   ########.fr       */
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

void	convert_args_to_int(char *av[], t_args *arguments, int ac)
{
	arguments->nbr_of_philos = ft_atoi(av[1]);
	arguments->time_to_die = ft_atoi(av[2]);
	arguments->time_to_eat = ft_atoi(av[3]);
	arguments->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		arguments->nbr_of_meals = ft_atoi(av[5]);
	return ;
}

int main(int ac, char *av[])
{
	t_args			arguments;
	pthread_mutex_t	*forks;
	
	if (ac < 5 || ac > 6)
	{
		write(2, "Incorrect number of arguments\n", 30);
		return (1);
	}
	convert_args_to_int(av, &arguments, ac);
	if (!manage_forks(&forks, arguments.nbr_of_philos));
		return (1);
	// Create threads/philosophers on a while loop
}

// Things to free/destroy:
// -forks pointer
// -Each mutex
