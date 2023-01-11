#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h> //remv
# include <sys/time.h>

typedef struct s_menu	t_menu;

typedef enum e_ingredients
{
	NO_PHIL = 1,
	TT_DIE = 2,
	TT_EAT = 3,
	TT_SLP = 4,
	NO_MEALS = 5,
}	t_ingredients;

typedef struct s_philos
{
	pthread_t		thread;
	pthread_mutex_t	*right;
	pthread_mutex_t	*left; // left is allocated last right

	bool				existence;
	int					id;
	unsigned long		last_meal;

	t_menu				*menu;
}	t_philos;

typedef struct s_menu
{
	t_philos	*philos;
	unsigned long	clock;
	int				no_phls;
	int				ttd;
	int				tte;
	int				tts;
	int				meals;
}	t_menu;

typedef struct s_fork
{
	int id;
	pthread_mutex_t lock;
}	t_fork;

int parse_args(char	**args);

#endif
