#include "philo.h"

static int	start_threads(t_table *table)
{
	int	i;

	table->start = now_ms() + 10 + table->count / 10;
	i = 0;
	while (i < table->count)
	{
		table->philos[i].last_meal = table->start;
		if (pthread_create(&table->philos[i].thread, NULL, philo_routine,
				&table->philos[i]))
		{
			printf("philo: could not create thread %d\n", i + 1);
			set_stopped(table);
			return (i);
		}
		i++;
	}
	return (i);
}

int	main(int argc, char **argv)
{
	t_table	table;
	int		created;
	int		i;

	if (!parse_args(&table, argc, argv))
		return (1);
	if (table.must_eat == 0)
		return (0);
	if (!table_init(&table))
	{
		printf("philo: initialisation failed\n");
		return (1);
	}
	created = start_threads(&table);
	if (created == table.count)
		monitor(&table);
	i = 0;
	while (i < created)
		pthread_join(table.philos[i++].thread, NULL);
	table_destroy(&table);
	return (created != table.count);
}
