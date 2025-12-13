/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:12:19 by elara-va          #+#    #+#             */
/*   Updated: 2025/12/13 17:17:02 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

// The mutexes must be unlocked
void	destroy_forks(pthread_mutex_t *forks, int nbr_of_forks)
{
	int	i;

	i = 0;
	while (i < nbr_of_forks)
		pthread_mutex_destroy(&(forks[i++]));
	free(forks);
	return ;
}

// The pointer passed must be the head of the list
void	free_philos_list(t_philosophers *philosophers)
{
	t_philosophers	*tmp;

	while (philosophers->next != NULL)
	{
		tmp = philosophers;
		philosophers = philosophers->next;
		free(tmp);
	}
	free(philosophers);
	return ;
}
