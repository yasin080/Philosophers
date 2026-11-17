#include "philo.h"

long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

void	wait_until(long timestamp)
{
	while (now_ms() < timestamp)
		usleep(200);
}

/* Short usleep steps so a philosopher notices a death quickly. */
void	sleep_ms(t_table *table, long ms)
{
	long	end;

	end = now_ms() + ms;
	while (now_ms() < end && !is_stopped(table))
		usleep(500);
}
