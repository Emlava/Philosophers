/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:01:28 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/18 14:50:32 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char *av[])
{
	t_resources		resources;

	if (ac < 5 || ac > 6)
	{
		write(2, "Incorrect number of arguments\n", 30);
		return (1);
	}
	if (!convert_args_to_int(av, &resources, ac))
		return (2);
	if (resources.nbr_of_meals == 0)
		return (0);
	if (!allocate_philos_list(&resources.philosopher_list,
			resources.requested_philos))
		return (3);
	if (!create_forks(&resources))
		return (4);
	if (!create_locks(&resources))
		return (5);
	if (!manage_simulation(&resources))
	{
		write(2, "Failed to create thread\n", 24);
		return (6);
	}
	return (0);
}

// Things to free/destroy and threads to join/detach:
// -Each mutex (forks and locks)
// -forks pointer
// -Each thread (detach/join)
// -thread list (free)
// Join/detach philosopher_list
// Join/detach monitor
