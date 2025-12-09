/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:55:58 by elara-va          #+#    #+#             */
/*   Updated: 2025/12/07 15:57:00 by elara-va         ###   ########.fr       */
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

int	convert_args_to_int(char *av[], t_resources *resources, int ac)
{
	resources->nbr_of_philos = ft_atoi(av[1]);
	if (resources->nbr_of_philos <= 0)
		return (0);
	resources->time_to_die = ft_atoi(av[2]);
	resources->time_to_eat = ft_atoi(av[3]);
	resources->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		resources->nbr_of_meals = ft_atoi(av[5]);
	else
		resources->nbr_of_meals = -1;
	if (resources->nbr_of_philos <= 0 || resources->time_to_die < 0
		|| resources->time_to_eat < 0 || resources->time_to_sleep < 0
		|| (ac == 6 && resources->nbr_of_meals <= 0))
		return (0);
	else
		return (1);
}

int	create_forks(pthread_mutex_t **forks, int nbr_of_forks)
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
			destroy_forks(*forks, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	allocate_thread_list(t_threads **threads, int nbr_of_philos)
{
	t_threads	*threads_cpy;
	int	i;

	*threads = malloc(sizeof(t_threads));
	if (!(*threads))
		return (0);
	(*threads)->next = NULL;
	threads_cpy = *threads;
	i = 1;
	while (i++ < nbr_of_philos)
	{
		threads_cpy->next = malloc(sizeof(t_threads));
		if (!threads_cpy->next)
		{
			threads_cpy->next = NULL;
			free_thread_list(*threads);	
			return (0);
		}
		threads_cpy = threads_cpy->next;
	}
	threads_cpy->next = NULL;
	return (1);
}
