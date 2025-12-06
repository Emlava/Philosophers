/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:12:19 by elara-va          #+#    #+#             */
/*   Updated: 2025/12/06 19:21:53 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
void	free_thread_list(t_threads *threads)
{
	t_threads	*tmp;
	
	while (threads->next != NULL)
	{
		tmp = threads;
		threads = threads->next;
		free(tmp);
	}
	free(threads);
	return ;
}

void	clean_up(int to_clean, ...)
{
	va_list	arg_list;

	va_start(arg_list, to_clean);
	

	va_end(arg_list);
}
