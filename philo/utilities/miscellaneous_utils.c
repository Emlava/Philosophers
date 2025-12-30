/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miscellaneous_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:01:23 by elara-va          #+#    #+#             */
/*   Updated: 2025/12/21 20:17:30 by elara-va         ###   ########.fr       */
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

static long	time_stamp_ms(struct timeval initial_time, struct timeval current_time)
{
	long long	initial_microseconds;
	long long	current_microseconds;

	initial_microseconds = (initial_time.tv_sec * 1000000) + initial_time.tv_usec;
	current_microseconds = (current_time.tv_sec * 1000000) + current_time.tv_usec;
	return ((long)((current_microseconds - initial_microseconds) / 1000));
}

int	print_state_change(t_resources *resources, int philosopher, char *new_state)
{
	struct timeval			current_time;
	static struct timeval	previous_time;

	pthread_mutex_lock(&resources->print_lock);
	if (resources->stop_flag == 1)
	{
		pthread_mutex_unlock(&resources->print_lock);
		return (0);
	}
	gettimeofday(&current_time, NULL);
	// WE LEFT OFF HERE
	// Here we check that time_to_die has not passed since the initial time or the timestamp of the last meal.
	// previous_time has to be equal to initial time if the first meal has not been had
	// or to current_time (below) if it has.
	// If time_to_die has passed, we assign 1 to all_meals_or_death_flag and print the death
	printf("%ldms %d %s\n", time_stamp_ms(resources->initial_time, current_time), philosopher, new_state);
	previous_time = current_time;
	pthread_mutex_unlock(&resources->print_lock);
	return (1);
}
