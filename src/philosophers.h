#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h> //remve

typedef struct s_menu	t_menu;

typedef struct s_args
{
	int	phils;
	int	ttd;
	int	tte;
	int	tts;
}	t_args;

typedef struct s_philos
{
	pthread_t		thread;
	pthread_mutex_t	*right;
	pthread_mutex_t	*left; // left is allocated last right

	bool				state;
	int					id;
	unsigned long		last_meal;

	t_menu				*menu;
}	t_philos;

typedef struct s_menu
{
	t_philos	*philos;
	int			meals;
	t_args		*s_args;
}	t_menu;

typedef struct s_fork
{
	int id;
	pthread_mutex_t lock;
}	t_fork;

int parse_args(char	**args);

#endif
