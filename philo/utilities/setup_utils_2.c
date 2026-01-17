/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 14:25:48 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/17 14:29:05 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void	assign_lock_arr_elements(pthread_mutex_t **lock_arr,
	t_resources *resources)
{
	lock_arr[0] = &resources->ect_flag_lock;
	lock_arr[1] = &resources->node_ready_flag_lock;
	lock_arr[2] = &resources->start_simulation_flag_lock;
	lock_arr[3] = &resources->odds_meal_count_lock;
	lock_arr[4] = &resources->evens_meal_count_lock;
	lock_arr[5] = &resources->odds_eat_flag_lock;
	lock_arr[6] = &resources->evens_eat_flag_lock;
	lock_arr[7] = &resources->print_lock;
	lock_arr[8] = &resources->full_philos_lock;
	lock_arr[9] = &resources->stop_flag_lock;
	return ;
}

int	create_locks(t_resources *resources)
{
	pthread_mutex_t		*lock_arr[10];
	int					i;

	assign_lock_arr_elements(lock_arr, resources);
	i = 0;
	while (i <= 9)
	{
		if (pthread_mutex_init(lock_arr[i], NULL) != 0)
			break ;
		i++;
	}
	if (i == 10)
		return (1);
	else
	{
		free_philos_list(resources->philosopher_list);
		destroy_forks(resources->forks, resources->requested_philos);
		while (i--)
			pthread_mutex_destroy(lock_arr[i]);
		return (0);
	}
}
