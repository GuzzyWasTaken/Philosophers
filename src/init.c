/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/16 16:50:10 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/01/18 15:13:36 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdbool.h>

bool	ft_isdigit(char a)
{
	if ((a >= '0') && (a <= '9'))
		return (true);
	else
		return (false);
}


int	ft_atoi(const char	*s)
{
	int				i;
	unsigned long	num;

	i = 1;
	num = 0;
	while (*s == '\n' || *s == '\t' || *s == '\r' || *s == '\n'
		|| *s == '\v' || *s == '\f' || *s == ' ')
		s++;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			i = -1;
		s++;
	}
	while ((*s >= '0') && (*s <= '9'))
	{
		num = (num * 10) + (*s - '0');
		s++;
	}
	if (num > 9223372036854775807)
		return (0);
	return ((int)num * i);
}

unsigned long	get_time(t_menu *menu)
{
	struct timeval	tv;
	unsigned long	curtime;

	gettimeofday(&tv, NULL);
	curtime = (tv.tv_sec * 1000);
	curtime += (tv.tv_usec / 1000);
	return (curtime - menu->start);
}

void	terminate(t_philos *philo)
{
	pthread_mutex_lock(&philo->menu->master_lock);
	if (philo->menu->terminate == true)
		philo->existence = false;
	pthread_mutex_unlock(&philo->menu->master_lock);
}

int	report(t_philos	*philo, unsigned long time)
{
	pthread_mutex_lock(&philo->menu->report_lock);
	if (philo->menu->death_counter == 1)
		return (0);
	terminate(philo);
	if (philo->existence == false)
	{
		philo->menu->death_counter = 1;
		printf(DEATH_T, time, philo->id);
		pthread_mutex_unlock(&philo->menu->report_lock);
		return (1);
	}
	if (philo->state == EATING)
		printf(EATING_T, time, philo->id);
	if (philo->state == SLEEPING)
		printf(SLEEPING_T, time, philo->id);
	if (philo->state == THINKING)
		printf(THINKING_T, time, philo->id);
	if (philo->state == FORK_L)
		printf(LFORK_T, time, philo->id);
	if (philo->state == FORK_R)
		printf(RFORK_T, time, philo->id);
	pthread_mutex_unlock(&philo->menu->report_lock);
	return (0);
}


void	observe(t_menu *menu)
{
	int				i;
	unsigned long	time;
	t_philos		*phil;

	i = 0;
	phil = menu->philos;
	pthread_mutex_lock(&menu->master_lock);
	pthread_mutex_unlock(&menu->master_lock);
	while(menu->terminate != true)
	{
		while (i < menu->no_phls)
		{
			pthread_mutex_lock(&phil[i].body);
			time = get_time(menu);
			if (time - phil[i].last_meal > menu->ttd)
			{
				menu->terminate = true;
				if (report(&phil[i], get_time(phil[i].menu)) == 1)
				{
					pthread_mutex_lock(&phil[i].menu->report_lock);
				}
				// printf ("test %lu time = %lu last = %lu\n", time - phil[i].last_meal, time, phil[i].last_meal);
				// printf("loser %i has died loool at %lu last meal at %lu\n", phil[i].id, time, phil[i].last_meal);
				break;
			}
			pthread_mutex_unlock(&phil[i].body);
			i++;
		}
		usleep(500);
		i = 0;
	}
}


void	prep(t_philos *philo)
{
	t_menu	*menu;

	menu = philo->menu;
	philo->existence = true;
	philo->state = EATING;
	// pthread_mutex_lock(&menu->master_lock);
	// pthread_mutex_unlock(&menu->master_lock);
	philo->last_meal = get_time(menu);
	if (philo->id % 2 == 0)
		usleep(500);
}

void	dindins(void	*param)
{
	t_philos	*philo;
	t_menu		*menu;
	unsigned long	time;

	philo = param;
	menu = philo->menu;
	philo->state = FORK_R;
	pthread_mutex_lock(philo->right);
	report(philo, get_time(philo->menu));
	philo->state = FORK_L;
	if (!philo->left)
		return ;
	pthread_mutex_lock(philo->left);
	report(philo, get_time(philo->menu));
	pthread_mutex_lock(&philo->body);
	philo->state = EATING;
	report(philo, get_time(philo->menu));
	philo->last_meal = get_time(menu);
	pthread_mutex_unlock(&philo->body);
	usleep(menu->tte * 1000);
	pthread_mutex_unlock(philo->right);
	pthread_mutex_unlock(philo->left);
	philo->state = SLEEPING;
}

void	*birth(void	*param)
{
	t_philos	*philo;
	t_menu		*menu;

	philo = param;
	menu = philo->menu;
	prep(philo);
	while (philo->existence == true)
	{
		if (philo->state == EATING)
			dindins(philo);
		if (philo->state == SLEEPING)
		{
			usleep(philo->menu->tts * 1000);
			philo->state = THINKING;
		}
		if (philo->state == THINKING)
		{
			report(philo, get_time(philo->menu));
			philo->state = EATING;
		}
		terminate(philo);
	}
	return (NULL);
}

void	create_phils(t_menu	*menu)
{
	int	i;

	i = -1;
	while (++i < menu->no_phls)
		pthread_create(&menu->philos[i].thread, NULL, &birth, &menu->philos[i]);
	observe(menu);
	pthread_mutex_unlock(&menu->master_lock);
	i = -1;
	while (++i < menu->no_phls)
		pthread_join(menu->philos[i].thread, NULL);
	/* join at end in sep func*/
}

int	prepare(t_menu	*menu)
{
	int	i;

	i = -1;
	while (++i < menu->no_phls)
	{
		menu->philos[i].last_meal = 0;
		menu->philos[i].id = i + 1;
		menu->philos[i].right = malloc(1 * sizeof(pthread_mutex_t));
		menu->philos[i].menu = menu;
		pthread_mutex_init(&menu->philos[i].body, NULL);
		if (pthread_mutex_init(menu->philos[i].right, NULL)!= 0)
			return (1);
		if (i != 0)
			menu->philos[i].left = menu->philos[i - 1].right;
	}
	if (menu->no_phls == 1)
		menu->philos[i].left = NULL;
	menu->philos[0].left = menu->philos[i - 1].right;
	return (0);
}

/*still needs proper error handling or parsing
needs time of day*/
int	init(char	**args)
{
	t_menu	*menu;
	t_fork	*forks;

	menu = calloc(1, sizeof(t_menu));
	if (!menu)
		return (1);
	menu->no_phls = ft_atoi(args[NO_PHIL]);
	menu->ttd = ft_atoi(args[TT_DIE]);
	menu->tte = ft_atoi(args[TT_EAT]);
	menu->tts = ft_atoi(args[TT_SLP]);
	menu->philos = malloc(menu->no_phls * sizeof(t_philos));
	if (!menu->philos)
		return (1);
	menu->terminate = false;
	menu->start = get_time(menu);
	prepare(menu);
	create_phils(menu);
	return (0);
}

int	parse(char	**av)
{
	int	i;
	int	x;
	
	i = 0;
	x = 0;
	while (av[++i])
	{
		while(av[i][x])
		{
			if (ft_isdigit(av[i][x] == false || av[i][x] < 0)
				return (1);
			x++;
		}
		x = 0;
	}
	return (0);
}