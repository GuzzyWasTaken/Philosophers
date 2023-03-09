#include "../includes/philosophers.h"

void	prep(t_philos *philo)
{
	t_menu	*menu;

	menu = philo->menu;
	philo->existence = true;
	philo->state = EATING;
	pthread_mutex_lock(&menu->master_lock);
	pthread_mutex_unlock(&menu->master_lock);
	pthread_mutex_lock(&philo->body);
	philo->last_meal = get_time(menu);
	pthread_mutex_unlock(&philo->body);
	if (philo->id % 2 == 0)
		usleep(500);
}

int	dindins(void	*param)
{
	t_philos		*philo;
	t_menu			*menu;

	philo = param;
	menu = philo->menu;
	philo->state = FORK_R;
	pthread_mutex_lock(philo->right);
	report(philo, get_time(philo->menu));
	philo->state = FORK_L;
	if (philo->menu->no_phls == 1)
		return (0);
	pthread_mutex_lock(philo->left);
	report(philo, get_time(philo->menu));
	philo->state = EATING;
	pthread_mutex_lock(&philo->body);
	philo->num_meals++;
	report(philo, get_time(philo->menu));
	philo->last_meal = get_time(menu);
	pthread_mutex_unlock(&philo->body);
	better_sleep(menu->tte, menu);
	pthread_mutex_unlock(philo->right);
	pthread_mutex_unlock(philo->left);
	philo->state = SLEEPING;

	return (0);
}

//make local existnce bool
void	*birth(void	*param)
{
	t_philos	*philo;
	int			state;

	philo = param;
	prep(philo);
	state = terminate(philo);
	while (state == ONGOING)
	{
		if (philo->state == EATING)
			dindins(philo);
		else if (philo->state == SLEEPING)
		{
			state = terminate(philo);
			better_sleep(philo->menu->tts, philo->menu);
			pthread_mutex_lock(&philo->body);
			report(philo, get_time(philo->menu));
			pthread_mutex_unlock(&philo->body);
			philo->state = THINKING;
		}
		else if (philo->state == THINKING)
		{
			state = terminate(philo);
			pthread_mutex_lock(&philo->body);
			report(philo, get_time(philo->menu));
			pthread_mutex_unlock(&philo->body);
			philo->state = EATING;
		}
		state = terminate(philo);
		// printf("philo %i world state is %i\n",philo->id, state);
	}
	return (NULL);
}

