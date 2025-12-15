/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:55:58 by elara-va          #+#    #+#             */
/*   Updated: 2025/12/15 20:23:13 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static size_t	ft_if_space_or_sign(const char *nptr)
{
	size_t	i;

	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
		i++;
	return (i);
}

static int	ft_atoi(const char *nptr)
{
	size_t	i;
	int		minus;
	long	result;

	minus = 0;
	result = 0;
	i = ft_if_space_or_sign(nptr);
	if (i && nptr[i - 1] == '-')
		minus = 1;
	if (nptr[i] < '0' || nptr[i] > '9')
		return (0);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	if (minus)
		result = -result;
	return ((int)result);
}

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

int	create_forks(t_resources *resources)
{
	int	i;

	resources->forks = malloc(sizeof(pthread_mutex_t) * resources->requested_philos);
	if (!resources->forks)
		return (0);
	i = 0;
	while (i < resources->requested_philos)
	{
		if (pthread_mutex_init(&resources->forks[i], NULL) != 0)
		{
			destroy_forks(resources->forks, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	create_locks(t_resources *resources)
{
	int	return_value;

	return_value = 1;
	if (pthread_mutex_init(&resources->philo_nbr_lock, NULL) != 0)
		return_value = 0;
	if (return_value != 0)
	{
		if (pthread_mutex_init(&resources->print_lock, NULL) != 0)
		{
			return_value = 0;
			pthread_mutex_destroy(&resources->philo_nbr_lock);
		}
	}
	if (return_value == 0)
		destroy_forks(resources->forks, resources->requested_philos);
	return (return_value);
}

int	allocate_philos_list(t_philosophers **philosophers, int requested_philos)
{
	t_philosophers	*philosophers_cpy;
	int	i;

	*philosophers = malloc(sizeof(t_philosophers));
	if (!(*philosophers))
		return (0);
	(*philosophers)->next = NULL;
	philosophers_cpy = *philosophers;
	i = 1;
	while (i++ < requested_philos)
	{
		philosophers_cpy->next = malloc(sizeof(t_philosophers));
		if (!philosophers_cpy->next)
		{
			philosophers_cpy->next = NULL;
			free_philos_list(*philosophers);	
			return (0);
		}
		philosophers_cpy = philosophers_cpy->next;
	}
	philosophers_cpy->next = NULL;
	return (1);
}
