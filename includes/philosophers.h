/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/07 15:48:55 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/03/21 20:21:25 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

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
# define DEATH_T  "%lu Philosopher %i has died\n"
//# define LFORK_T "%lu Philosopher %i has picked up Left fork\n"
//# define RFORK_T "%lu Philosopher %i has picked up Right fork\n"
# define LFORK_T "%lu Philosopher %i has picked up a fork\n"
# define RFORK_T "%lu Philosopher %i has picked up a fork\n"

typedef struct s_menu	t_menu;

typedef enum e_state
{
	THINKING = 1,
	EATING = 2,
	SLEEPING = 3,
	FORK_L = 4,
	FORK_R = 5,
}	t_state;

typedef enum e_term_state
{
	ONGOING = 1,
	TERMINATE = 2,
	SATISFIED = 3,
}	t_term_state;

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
	long			no_phls;
	long			ttd;
	long			tte;
	long			tts;
	long			meals;
	long			death_counter;
	pthread_mutex_t term_lock;
	pthread_mutex_t	master_lock;
	pthread_mutex_t report_lock;
	pthread_mutex_t fat_lock;
	int				terminate;
}	t_menu;

typedef struct s_fork
{
	int id;
	pthread_mutex_t lock;
}	t_fork;

int				parse_args(char	**args, t_menu *menu);
int				parse(char	**av);
int				prepare(t_menu	*menu);
void			create_phils(t_menu	*menu);
void			join_threads(t_menu *menu);
void			*birth(void	*param);
int				dindins(void	*param);
void			prep(t_philos *philo);
void			observe(t_menu *menu);
int				last_supper(t_menu *menu);
int				report(t_philos	*philo, unsigned long time);
int				terminate(t_philos *philo);
void			fax_report(t_philos *philo);
bool			done_eating(t_menu *menu);
unsigned long	get_time(t_menu *menu);
int				ft_atoi(const char	*s);
bool			ft_isdigit(char a);
void			better_sleep(unsigned long duration, t_menu *menu);
int				init_mutex(t_menu *menu);

#endif
