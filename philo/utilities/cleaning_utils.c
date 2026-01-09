/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:12:19 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/09 18:38:16 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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
void	free_philos_list(t_philosopher_list *philosopher_list)
{
	t_philosopher_list	*tmp;

	while (philosopher_list->next != NULL)
	{
		tmp = philosopher_list;
		philosopher_list = philosopher_list->next;
		free(tmp);
	}
	free(philosopher_list);
	return ;
}
