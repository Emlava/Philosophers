/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:12:19 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/17 16:19:57 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

// The pointer passed must be the head of the list
void	free_philos_list(t_philosopher_list *philosopher_list)
{
	t_philosopher_list	*tmp;

	while (philosopher_list != NULL)
	{
		pthread_mutex_destroy(&philosopher_list->pmits_lock);
		tmp = philosopher_list;
		philosopher_list = philosopher_list->next;
		free(tmp);
	}
	return ;
}

void	destroy_forks(pthread_mutex_t *forks, int nbr_of_forks)
{
	int	i;

	i = 0;
	while (i < nbr_of_forks)
		pthread_mutex_destroy(&(forks[i++]));
	free(forks);
	return ;
}

static void	destroy_locks(t_resources *resources)
{
	pthread_mutex_destroy(&resources->ect_flag_lock);
	pthread_mutex_destroy(&resources->node_ready_flag_lock);
	pthread_mutex_destroy(&resources->start_simulation_flag_lock);
	pthread_mutex_destroy(&resources->odds_meal_count_lock);
	pthread_mutex_destroy(&resources->evens_meal_count_lock);
	pthread_mutex_destroy(&resources->odds_eat_flag_lock);
	pthread_mutex_destroy(&resources->evens_eat_flag_lock);
	pthread_mutex_destroy(&resources->print_lock);
	pthread_mutex_destroy(&resources->full_philos_lock);
	pthread_mutex_destroy(&resources->stop_flag_lock);
	return ;
}

void	full_cleanup(t_resources *resources, int return_value,
	int created_philos, pthread_t monitor)
{
	resources->curr_philo_node = resources->philosopher_list;
	if (return_value == 1)
	{
		while (created_philos--)
		{
			pthread_join(resources->curr_philo_node->thread, NULL);
			resources->curr_philo_node = resources->curr_philo_node->next;
		}
		pthread_join(monitor, NULL);
	}
	else
	{
		while (created_philos--)
		{
			pthread_detach(resources->curr_philo_node->thread);
			resources->curr_philo_node = resources->curr_philo_node->next;
		}
	}
	free_philos_list(resources->philosopher_list);
	destroy_forks(resources->forks, resources->requested_philos);
	destroy_locks(resources);
	return ;
}
