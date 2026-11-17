#include "philo.h"

static bool	to_int(const char *s, long *out)
{
	long	n;

	if (*s == '+')
		s++;
	if (*s < '0' || *s > '9')
		return (false);
	n = 0;
	while (*s >= '0' && *s <= '9')
	{
		n = n * 10 + (*s++ - '0');
		if (n > 2147483647)
			return (false);
	}
	*out = n;
	return (*s == '\0');
}

static bool	usage(void)
{
	printf("usage: ./philo number_of_philosophers time_to_die time_to_eat "
		"time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
	return (false);
}

static bool	invalid(const char *arg)
{
	printf("philo: invalid argument: '%s' (expected a positive integer)\n",
		arg);
	return (false);
}

bool	parse_args(t_table *table, int argc, char **argv)
{
	long	values[5];
	int		i;

	if (argc != 5 && argc != 6)
		return (usage());
	values[4] = -1;
	i = 1;
	while (i < argc)
	{
		if (!to_int(argv[i], &values[i - 1]))
			return (invalid(argv[i]));
		if (i < 5 && values[i - 1] == 0)
			return (invalid(argv[i]));
		i++;
	}
	table->count = values[0];
	table->time_to_die = values[1];
	table->time_to_eat = values[2];
	table->time_to_sleep = values[3];
	table->must_eat = values[4];
	return (true);
}
