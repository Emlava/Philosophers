/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:55:58 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/17 14:34:32 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	convert_args_to_int(char *av[], t_resources *resources, int ac)
{
	resources->requested_philos = ft_atoi(av[1]);
	if (resources->requested_philos <= 0)
		return (0);
	resources->time_to_die = ft_atoi(av[2]);
	resources->time_to_eat = ft_atoi(av[3]);
	resources->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		resources->nbr_of_meals = ft_atoi(av[5]);
	else
		resources->nbr_of_meals = -1;
	if (resources->requested_philos <= 0 || resources->time_to_die < 0
		|| resources->time_to_eat < 0 || resources->time_to_sleep < 0
		|| (ac == 6 && resources->nbr_of_meals <= 0))
		return (0);
	else
		return (1);
}

static int	manage_philo_node(t_philosopher_list **philosopher_node, int i)
{
	if (i > 1)
	{
		(*philosopher_node)->next = malloc(sizeof(t_philosopher_list));
		if (!(*philosopher_node)->next)
			return (0);
		*philosopher_node = (*philosopher_node)->next;
	}
	(*philosopher_node)->philosopher = i;
	(*philosopher_node)->prev_meal_or_initial_ts.tv_sec = -1;
	if (pthread_mutex_init(&(*philosopher_node)->pmits_lock, NULL) != 0)
		return (0);
	return (1);
}

int	allocate_philos_list(t_philosopher_list **philosopher_list,
	int requested_philos)
{
	int					return_value;
	int					i;
	t_philosopher_list	*philosopher_list_cpy;

	return_value = 1;
	i = 1;
	*philosopher_list = malloc(sizeof(t_philosopher_list));
	if (!*philosopher_list)
		return (0);
	philosopher_list_cpy = *philosopher_list;
	if (!manage_philo_node(&philosopher_list_cpy, i))
		return_value = 0;
	while (i++ < requested_philos && return_value == 1)
	{
		if (!manage_philo_node(&philosopher_list_cpy, i))
			return_value = 0;
	}
	philosopher_list_cpy->next = NULL;
	if (!return_value)
		free_philos_list(*philosopher_list);
	return (return_value);
}

int	create_forks(t_resources *resources)
{
	int	i;

	resources->forks
		= malloc(sizeof(pthread_mutex_t) * resources->requested_philos);
	if (!resources->forks)
		return (0);
	i = 0;
	while (i < resources->requested_philos)
	{
		if (pthread_mutex_init(&resources->forks[i], NULL) != 0)
		{
			free_philos_list(resources->philosopher_list);
			destroy_forks(resources->forks, i);
			return (0);
		}
		i++;
	}
	return (1);
}
