/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/16 16:50:10 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/03/21 19:50:12 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	join_threads(t_menu *menu)
{
	int	i;

	i = -1;
	while (++i < menu->no_phls)
		pthread_join(menu->philos[i].thread, NULL);
}

void	create_phils(t_menu	*menu)
{
	int	i;

	i = -1;
	pthread_mutex_lock(&menu->master_lock);
	while (++i < menu->no_phls)
		pthread_create(&menu->philos[i].thread, NULL, &birth, &menu->philos[i]);
	pthread_mutex_unlock(&menu->master_lock);
}

int	init_mutex(t_menu *menu)
{
	int	error;

	error = 0;
	error += pthread_mutex_init(&menu->master_lock, NULL);
	error += pthread_mutex_init(&menu->report_lock, NULL);
	error += pthread_mutex_init(&menu->fat_lock, NULL);
	error += pthread_mutex_init(&menu->term_lock, NULL);
	if (error > 0)
		return (1);
	menu->philos = malloc(menu->no_phls * sizeof(t_philos));
	if (!menu->philos)
		return (1);
	return (0);
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
		if (pthread_mutex_init(menu->philos[i].right, NULL) != 0)
			return (1);
		if (i != 0)
			menu->philos[i].left = menu->philos[i - 1].right;
	}
	if (menu->no_phls == 1)
		menu->philos[i].left = NULL;
	menu->philos[0].left = menu->philos[i - 1].right;
	return (0);
}
