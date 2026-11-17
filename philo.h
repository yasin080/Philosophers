#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo	t_philo;

typedef struct s_table
{
	int				count;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat;
	long			start;
	bool			stop;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_table;

struct s_philo
{
	int				id;
	int				meals;
	long			last_meal;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	pthread_t		thread;
	t_table			*table;
};

/* parse.c */
bool	parse_args(t_table *table, int argc, char **argv);

/* init.c */
bool	table_init(t_table *table);
void	table_destroy(t_table *table);

/* routine.c */
void	*philo_routine(void *arg);

/* monitor.c */
void	monitor(t_table *table);

/* time.c */
long	now_ms(void);
void	wait_until(long timestamp);
void	sleep_ms(t_table *table, long ms);

/* print.c */
bool	is_stopped(t_table *table);
void	set_stopped(t_table *table);
void	print_state(t_philo *philo, const char *msg);
void	print_death(t_philo *philo);

#endif
