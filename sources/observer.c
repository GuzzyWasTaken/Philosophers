/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   observer.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/07 15:48:36 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/03/09 21:11:23 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	observe(t_menu *menu)
{
	t_philos		*phil;
	int				terminate;

	terminate = ONGOING;
	phil = menu->philos;
	while (terminate == ONGOING)
	{
		terminate = last_supper(menu);
		usleep(800);
	}
	return ;
}
int satisfaction(t_menu *menu, int	satisfied)
{
	if (satisfied == menu->no_phls && menu->meals > 0)
	{
		pthread_mutex_lock(&menu->term_lock);
		menu->terminate = SATISFIED;
		pthread_mutex_unlock(&menu->term_lock);
		return (SATISFIED);
	}
	return (ONGOING);
}

int	last_supper(t_menu *menu)
{
	t_philos	*phil;
	int			satisfied;
	int			i;

	i = -1;
	satisfied = 0;
	phil = menu->philos;
	while (++i < menu->no_phls)
	{
		pthread_mutex_lock(&phil[i].body);
		if (get_time(menu) - phil[i].last_meal > (unsigned long)menu->ttd)
		{
			phil[i].existence = false;
			pthread_mutex_lock(&menu->term_lock);
			menu->terminate = TERMINATE;
			pthread_mutex_unlock(&menu->term_lock);
			report(&phil[i], get_time(phil[i].menu));
			pthread_mutex_unlock(&phil[i].body);
			return (TERMINATE);
		}
		if (phil[i].num_meals >= menu->meals)
			satisfied++;
		pthread_mutex_unlock(&phil[i].body);
	}
	return (satisfaction(menu, satisfied));
}

int	terminate(t_philos *philo)
{
	pthread_mutex_lock(&philo->menu->term_lock);
	if (philo->menu->terminate == TERMINATE)
	{
		philo->existence = false;
		pthread_mutex_unlock(&philo->menu->term_lock);
		return (TERMINATE);
	}
	else if (philo->menu->terminate == SATISFIED)
	{
		pthread_mutex_unlock(&philo->menu->term_lock);
		return (SATISFIED);
	}
	pthread_mutex_unlock(&philo->menu->term_lock);
	return (true);
}