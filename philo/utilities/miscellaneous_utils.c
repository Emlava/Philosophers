/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miscellaneous_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:01:23 by elara-va          #+#    #+#             */
/*   Updated: 2025/12/21 18:01:26 by elara-va         ###   ########.fr       */
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

void	print_state_change(struct timeval initial_time, int philosopher, char *new_state, pthread_mutex_t *print_lock)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	printf("%ldms %d %s\n", time_stamp_ms(initial_time, current_time), philosopher, new_state);
	return ;
}
