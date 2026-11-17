#include "philo.h"

bool	is_stopped(t_table *table)
{
	bool	stopped;

	pthread_mutex_lock(&table->stop_lock);
	stopped = table->stop;
	pthread_mutex_unlock(&table->stop_lock);
	return (stopped);
}

void	set_stopped(t_table *table)
{
	pthread_mutex_lock(&table->stop_lock);
	table->stop = true;
	pthread_mutex_unlock(&table->stop_lock);
}

void	print_state(t_philo *philo, const char *msg)
{
	t_table	*table;

	table = philo->table;
	pthread_mutex_lock(&table->print_lock);
	if (!is_stopped(table))
		printf("%ld %d %s\n", now_ms() - table->start, philo->id, msg);
	pthread_mutex_unlock(&table->print_lock);
}

/*
** The stop flag is raised while holding print_lock, so no other message
** can be printed after "died".
*/
void	print_death(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	pthread_mutex_lock(&table->print_lock);
	if (!is_stopped(table))
	{
		set_stopped(table);
		printf("%ld %d died\n", now_ms() - table->start, philo->id);
	}
	pthread_mutex_unlock(&table->print_lock);
}
