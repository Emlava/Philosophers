#include "philo.h"

int	main(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	printf("|%ld| |%ld|\n", tv.tv_sec, tv.tv_usec);
	return (0);
}
