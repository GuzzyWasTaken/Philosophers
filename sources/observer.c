/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   observer.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/07 15:48:36 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/03/07 16:50:28 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	observe(t_menu *menu)
{
	t_philos		*phil;
	bool			fat;
	bool			terminate;

	terminate = false;
	fat = false;
	phil = menu->philos;
	while (terminate == false && fat == false)
	{
		terminate = last_supper(menu);
		usleep(500);
	}
	return ;
}

bool last_supper(t_menu *menu)
{
	t_philos	*phil;
	int			satisfied;
	int			i;

	i = 0;
	satisfied = 0;
	phil = menu->philos;
	while (i < menu->no_phls)
	{
		pthread_mutex_lock(&phil[i].body);
		if (get_time(menu) - phil[i].last_meal > (unsigned long)menu->ttd)
		{
			phil[i].existence = false;
			pthread_mutex_lock(&menu->master_lock);
			menu->terminate = true;
			pthread_mutex_unlock(&menu->master_lock);
			report(&phil[i], get_time(phil[i].menu));
			pthread_mutex_unlock(&phil[i].body);
			return (true);
		}
		if (phil[i].num_meals >= menu->meals)
			satisfied++;
		pthread_mutex_unlock(&phil[i].body);
		i++;
	}
	if (satisfied == menu->no_phls && menu->meals > 0)
	{
		pthread_mutex_lock(&menu->master_lock);
		menu->terminate = true;
		pthread_mutex_unlock(&menu->master_lock);
		return (true);
	}
	return (false);
}

bool	terminate(t_philos *philo)
{
	pthread_mutex_lock(&philo->menu->term_lock);
	if (philo->menu->terminate == true)
	{
		philo->existence = false;
		pthread_mutex_unlock(&philo->menu->term_lock);
		return (false);
	}
	pthread_mutex_unlock(&philo->menu->term_lock);
	return (true);
}