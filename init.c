#include "philo.h"

/*
** Odd philosophers pick the left fork first and even ones the right fork,
** so the "everyone holds one fork and waits" cycle can never close.
*/
static void	assign_forks(t_table *table, t_philo *philo, int i)
{
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;

	left = &table->forks[i];
	right = &table->forks[(i + 1) % table->count];
	if (philo->id % 2)
	{
		philo->first_fork = left;
		philo->second_fork = right;
	}
	else
	{
		philo->first_fork = right;
		philo->second_fork = left;
	}
}

static bool	init_mutexes(t_table *table)
{
	int	i;

	if (pthread_mutex_init(&table->stop_lock, NULL)
		|| pthread_mutex_init(&table->print_lock, NULL))
		return (false);
	i = 0;
	while (i < table->count)
	{
		if (pthread_mutex_init(&table->forks[i], NULL)
			|| pthread_mutex_init(&table->philos[i].meal_lock, NULL))
			return (false);
		i++;
	}
	return (true);
}

bool	table_init(t_table *table)
{
	int	i;

	table->stop = false;
	table->forks = malloc(sizeof(pthread_mutex_t) * table->count);
	table->philos = malloc(sizeof(t_philo) * table->count);
	if (!table->forks || !table->philos || !init_mutexes(table))
	{
		free(table->forks);
		free(table->philos);
		return (false);
	}
	i = 0;
	while (i < table->count)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meals = 0;
		table->philos[i].table = table;
		assign_forks(table, &table->philos[i], i);
		i++;
	}
	return (true);
}

void	table_destroy(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->count)
	{
		pthread_mutex_destroy(&table->forks[i]);
		pthread_mutex_destroy(&table->philos[i].meal_lock);
		i++;
	}
	pthread_mutex_destroy(&table->stop_lock);
	pthread_mutex_destroy(&table->print_lock);
	free(table->forks);
	free(table->philos);
}
