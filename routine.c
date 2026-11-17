#include "philo.h"

static void	eat(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	pthread_mutex_lock(philo->first_fork);
	print_state(philo, "has taken a fork");
	if (philo->first_fork == philo->second_fork)
	{
		sleep_ms(table, table->time_to_die * 2);
		pthread_mutex_unlock(philo->first_fork);
		return ;
	}
	pthread_mutex_lock(philo->second_fork);
	print_state(philo, "has taken a fork");
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = now_ms();
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_lock);
	print_state(philo, "is eating");
	sleep_ms(table, table->time_to_eat);
	pthread_mutex_unlock(philo->second_fork);
	pthread_mutex_unlock(philo->first_fork);
}

/*
** With an odd number of philosophers somebody is always waiting for a fork.
** Thinking a bit before trying again gives the hungriest neighbour the
** chance to eat first instead of grabbing the forks back immediately.
*/
static void	think(t_philo *philo)
{
	t_table	*table;
	long	think_time;

	table = philo->table;
	print_state(philo, "is thinking");
	if (table->count % 2 == 0)
		return ;
	think_time = table->time_to_eat * 2 - table->time_to_sleep;
	if (think_time > 0)
		sleep_ms(table, think_time / 2);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_table	*table;

	philo = arg;
	table = philo->table;
	wait_until(table->start);
	if (philo->id % 2 == 0)
		sleep_ms(table, table->time_to_eat / 2);
	while (!is_stopped(table))
	{
		eat(philo);
		print_state(philo, "is sleeping");
		sleep_ms(table, table->time_to_sleep);
		think(philo);
	}
	return (NULL);
}
