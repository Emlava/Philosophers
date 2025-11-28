/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elara-va <elara-va@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 20:55:38 by emlava            #+#    #+#             */
/*   Updated: 2025/11/28 16:45:33 by elara-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int ac, char *av[])
{
	pthread_mutex_t	*forks;
	
	if (ac < 5 || ac > 6)
	{
		write(2, "Incorrect number of arguments\n", 30);
		return (1);
	}
	forks = malloc(sizeof(pthread_mutex_t) * ft_atoi(av[1]));
}
