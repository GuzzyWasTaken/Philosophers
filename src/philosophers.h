#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>

typedef struct s_args
{
	int	start;
	int	num_of_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	is_dead;
}	t_args;

typedef struct s_philo
{
	int		meals;
	int		rf;
	int		lf;
	t_args	*s_args;
}	t_philo;

typedef struct s_fork
{
	int id;
	pthread_mutex_t lock;
}	t_fork;

int parse_args(char	**args);

#endif
