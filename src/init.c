/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/16 16:50:10 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/02/22 00:59:15 by auzochuk      ########   odam.nl         */
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




unsigned long	get_time(t_menu *menu)
{
	struct timeval	tv;
	unsigned long	curtime;

	gettimeofday(&tv, NULL);
	curtime = (tv.tv_sec * 1000);
	curtime += (tv.tv_usec / 1000);
	return (curtime - menu->start);
}

bool	done_eating(t_menu *menu)
{
	pthread_mutex_lock(&menu->fat_lock);
	int			i;
	int			count;
	t_philos	*phil;

	count = 0;
	i = 0;
	phil = menu->philos;
	if (menu->meals == 0)
	{
		pthread_mutex_unlock(&menu->fat_lock);
		return (false);
	}
	while (phil[i].num_meals >= menu->meals && i < menu->no_phls)
		i++;
	if (i == menu->no_phls)
	{
		pthread_mutex_unlock(&menu->fat_lock);
		return (true);
	}
	pthread_mutex_unlock(&menu->fat_lock);
	return (false);
}

bool	terminate(t_philos *philo)
{
	pthread_mutex_lock(&philo->menu->term_lock);
	if (philo->menu->terminate == true)
	{
		// printf("philo %i is false\n", philo->id);
		philo->existence = false;
		pthread_mutex_unlock(&philo->menu->term_lock);
		return (false);
	}
	pthread_mutex_unlock(&philo->menu->term_lock);
	return (true);
}


void	fax_report(t_philos *philo)
{
	if (philo->state == EATING)
		printf(EATING_T, get_time(philo->menu), philo->id);
	else if (philo->state == SLEEPING)
		printf(SLEEPING_T, get_time(philo->menu), philo->id);
	else if (philo->state == THINKING)
		printf(THINKING_T, get_time(philo->menu), philo->id);
	else if (philo->state == FORK_L)
		printf(LFORK_T, get_time(philo->menu), philo->id);
	else if (philo->state == FORK_R)
		printf(RFORK_T, get_time(philo->menu), philo->id);
}

int	report(t_philos	*philo, unsigned long time)
{

	pthread_mutex_lock(&philo->menu->report_lock);
	if (philo->menu->death_counter == 1)
	{
		terminate(philo);
		// pthread_mutex_unlock(&philo->menu->report_lock);
		//printf("%i is leaving report\n", philo->id);
		return (1);
	}
	terminate(philo);
	if (philo->existence == false)
	{
		philo->menu->death_counter = 1;
		printf(DEATH_T "with %i meals\n", time, philo->id, philo->num_meals);
		pthread_mutex_unlock(&philo->menu->report_lock);
		return (1);
	}
	else
		fax_report(philo);
	pthread_mutex_unlock(&philo->menu->report_lock);
	return (0);
}

bool last_supper(t_menu *menu)
{
	t_philos	*phil;
	int			i;

	i = 0;
	phil = menu->philos;
	while (i < menu->no_phls)
	{
		pthread_mutex_lock(&phil[i].body);
		if (get_time(menu) - phil[i].last_meal > menu->ttd) // DATA RACE?
		{
			//printf("philo %i 's last meal was %lu and the current time is %lu time to die is %i\n", phil->id, phil->last_meal, get_time(menu), phil->menu->ttd);
			phil[i].existence = false;
			pthread_mutex_lock(&menu->master_lock);
			menu->terminate = true;
			pthread_mutex_unlock(&menu->master_lock);
			report(&phil[i], get_time(phil[i].menu));
			pthread_mutex_unlock(&phil[i].body);
			return (true);
			// break ;
		}
		pthread_mutex_unlock(&phil[i].body);
		i++;
	}
	return (false);
}

void	observe(t_menu *menu)
{
	t_philos		*phil;
	bool			fat;
	bool			terminate;

	terminate = false;
	fat = false;
	phil = menu->philos;
	while (terminate == false && fat == false) // data race
	{
		fat = done_eating(menu);
		terminate = last_supper(menu);
		usleep(500);
	}
	return ;
}

void	prep(t_philos *philo)
{
	t_menu	*menu;

	menu = philo->menu;
	philo->existence = true;
	philo->state = EATING;
	pthread_mutex_lock(&menu->master_lock);
	pthread_mutex_unlock(&menu->master_lock);
	philo->last_meal = get_time(menu);
	if (philo->id % 2 == 0)
		usleep(500);
}

int	dindins(void	*param)
{
	t_philos		*philo;
	t_menu			*menu;
	unsigned long	time;

	philo = param;
	menu = philo->menu;
	philo->state = FORK_R;
	pthread_mutex_lock(philo->right);
	report(philo, get_time(philo->menu));
	philo->state = FORK_L;
	if (!philo->left)
		return (0) ;
	pthread_mutex_lock(philo->left);
	report(philo, get_time(philo->menu));
	pthread_mutex_lock(&philo->body);
	philo->state = EATING;
	philo->num_meals++;
	report(philo, get_time(philo->menu));
	philo->last_meal = get_time(menu);
	pthread_mutex_unlock(&philo->body);
	usleep(menu->tte * 1000);
	pthread_mutex_unlock(philo->right);
	pthread_mutex_unlock(philo->left);
	// printf("return is = %i\n");
	return (0);
}

//make local existnce bool

void	better_sleep(unsigned long duration, t_menu *menu)
{
	unsigned long	goal;
	unsigned long	now;
	unsigned long	delta;

	goal = get_time(menu) + duration;
	while (true)
	{
		now = get_time(menu);
		if (now >= goal)
			return ;
		delta = goal - now;
		if (delta <= 1)
		{
			while (get_time(menu) < goal)
				usleep(200);
			return ;
		}
		usleep(delta >> 1);
	}
}

void	*birth(void	*param)
{
	t_philos	*philo;
	t_menu		*menu;
	bool		existence;
	bool		fat;

	fat = false;
	philo = param;
	menu = philo->menu;
	prep(philo);
	existence = terminate(philo);
	while (existence == true && fat == false)
	{
		if (philo->state == EATING)
		{
			dindins(philo);
			philo->state = SLEEPING;
		}
		else if (philo->state == SLEEPING)
		{
			usleep(philo->menu->tts * 1000);
			// better_sleep(philo->menu->tts * 1000, menu); 
			philo->state = THINKING;
		}
		else if (philo->state == THINKING)
		{
			// existence = terminate(philo);
			report(philo, get_time(menu));
			philo->state = EATING;
		}
		//printf("philo %i in birth still\n", philo->id);
		existence = terminate(philo);
		fat = done_eating(menu);
		// if (fat == true)
		// 	printf("I KNOW WE'RE FAT %i\n", philo->id);
	}
	//printf("philo %i returning\n", philo->id);
	return (NULL);
}

void	join_threads(t_menu *menu)
{
	int	i;

	i = -1;
	while (++i < menu->no_phls)
		pthread_join(menu->philos[i].thread, NULL);
	return ;
}

void	create_phils(t_menu	*menu)
{
	int	i;
	i = -1;
	pthread_mutex_lock(&menu->master_lock);
	while (++i < menu->no_phls)
		pthread_create(&menu->philos[i].thread, NULL, &birth, &menu->philos[i]);
	pthread_mutex_unlock(&menu->master_lock);
	return ;
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
		menu->philos[i].num_meals = 0;
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

int	parse(char	**av)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	while (av[++i])
	{
		while (av[i][x])
		{
			if (ft_isdigit(av[i][x]) == false || av[i][x] < 0)
			{
				write(1, "invalid arguements: expected numeric input\n", 44);
				return (1);
			}
			x++;
		}
		x = 0;
	}
	return (0);
}
