/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 20:55:38 by emlava            #+#    #+#             */
/*   Updated: 2025/11/29 12:55:09 by elara-va         ###   ########.fr       */
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

int main(int ac, char *av[])
{
	pthread_mutex_t	*forks;
	
	if (ac < 5 || ac > 6)
	{
		write(2, "Incorrect number of arguments\n", 30);
		return (1);
	}
	if (!manage_forks(&forks, ft_atoi(av[1])))
		return (1);
}

// Things to free/destroy:
// -forks pointer
// -Each mutex
