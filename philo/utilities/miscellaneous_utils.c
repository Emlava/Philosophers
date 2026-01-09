/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miscellaneous_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:01:23 by elara-va          #+#    #+#             */
/*   Updated: 2026/01/09 19:22:14 by elara-va         ###   ########.fr       */
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

int	ft_atoi(const char *nptr)
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

long	get_time_interval_ms(struct timeval first_ts, struct timeval second_ts)
{
	long long	first_ts_in_ms;
	long long	second_ts_in_ms;

	first_ts_in_ms = (first_ts.tv_sec * 1000000) + first_ts.tv_usec;
	second_ts_in_ms = (second_ts.tv_sec * 1000000) + second_ts.tv_usec;
	return ((long)((second_ts_in_ms - first_ts_in_ms) / 1000));
}

static int	philo_strcmp(const char *s1, const char *s2)
{
	const char	*p1;
	const char	*p2;

	p1 = s1;
	p2 = s2;
	while (*p1 && *p1 == *p2)
	{
		p1++;
		p2++;
	}
	if (!*p1 && !*p2)
		return (1);
	else
		return (0);
}

int	print_state_change(t_resources *resources, char *new_state,
	t_philosopher_list *philosopher_node)
{
	struct timeval			current_time;

	if (resources->stop_flag == 1) // Is this necessary here as well?
		return (0);
	pthread_mutex_lock(&resources->print_lock);
	gettimeofday(&current_time, NULL);
	if (philo_strcmp(new_state, E) == 1)
		philosopher_node->prev_meal_or_initial_ts = current_time;
	if (resources->stop_flag == 1)
	{
		pthread_mutex_unlock(&resources->print_lock);
		return (0);
	}
	printf("%ldms %d %s\n", get_time_interval_ms(resources->initial_time, current_time), 
		philosopher_node->philosopher, new_state);
	pthread_mutex_unlock(&resources->print_lock);
	return (1);
}
