/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   report.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/07 15:49:02 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/03/21 20:14:55 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

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
	int	state;

	state = terminate(philo);
	pthread_mutex_lock(&philo->menu->report_lock);
	if (philo->menu->death_counter == 1 || state == SATISFIED)
	{
		pthread_mutex_unlock(&philo->menu->report_lock);
		return (1);
	}
	if (state == TERMINATE)
	{
		philo->menu->death_counter = 1;
		printf(DEATH_T, time, philo->id);
		pthread_mutex_unlock(&philo->menu->report_lock);
		return (1);
	}
	else
		fax_report(philo);
	pthread_mutex_unlock(&philo->menu->report_lock);
	return (0);
}
