#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h> //remv
# include <sys/time.h>
# include <limits.h>

# define EATING_T  "%lu Philosopher %i is now eating\n"
# define SLEEPING_T  "%lu Philosopher %i is now sleeping\n"
# define THINKING_T  "%lu Philosopher %i is now thinking\n"
# define DEATH_T  "%lu Philoser %i has died\n"
# define LFORK_T "%lu Philosopher %i has picked up Left fork\n"
# define RFORK_T "%lu Philosopher %i has picked up Right fork\n"

typedef struct s_menu	t_menu;

typedef enum e_state
{
	THINKING = 1,
	EATING = 2,
	SLEEPING = 3,
	FORK_L = 4,
	FORK_R = 5,
}	t_state;

typedef enum e_ingredients
{
	NO_PHIL = 1,
	TT_DIE = 2,
	TT_EAT = 3,
	TT_SLP = 4,
	NO_MEALS = 5,
}	t_ingredients;

typedef struct s_watcher
{
	pthread_t	thread;
	t_menu 		*menu;
} t_watcher;

typedef struct s_philos
{
	pthread_t		thread;
	pthread_mutex_t	*right;
	pthread_mutex_t	*left; // left is allocated last right
	pthread_mutex_t	body;

	bool				existence;
	int					state;
	int					id;
	unsigned long		last_meal;
	int					num_meals;

	t_menu				*menu;
}	t_philos;

typedef struct s_menu
{
	t_philos		*philos;
	unsigned long	start;
	int				no_phls;
	int				ttd;
	int				tte;
	int				tts;
	int				meals;
	int				death_counter;
	pthread_mutex_t	master_lock;
	pthread_mutex_t report_lock;
	bool			terminate;
	bool			fat;
}	t_menu;

typedef struct s_fork
{
	int id;
	pthread_mutex_t lock;
}	t_fork;

int	init(char	**args);
int	parse(char	**av);

#endif
