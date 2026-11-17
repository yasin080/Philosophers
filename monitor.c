#include "philo.h"

static bool	check_philo(t_philo *philo, int *full)
{
	t_table	*table;
	long	last_meal;
	int		meals;

	table = philo->table;
	pthread_mutex_lock(&philo->meal_lock);
	last_meal = philo->last_meal;
	meals = philo->meals;
	pthread_mutex_unlock(&philo->meal_lock);
	if (now_ms() - last_meal >= table->time_to_die)
	{
		print_death(philo);
		return (true);
	}
	if (table->must_eat > 0 && meals >= table->must_eat)
		(*full)++;
	return (false);
}

void	monitor(t_table *table)
{
	int	i;
	int	full;

	wait_until(table->start);
	while (!is_stopped(table))
	{
		full = 0;
		i = 0;
		while (i < table->count)
		{
			if (check_philo(&table->philos[i], &full))
				return ;
			i++;
		}
		if (table->must_eat > 0 && full == table->count)
		{
			set_stopped(table);
			return ;
		}
		usleep(500);
	}
}
