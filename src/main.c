#include "philosophers.h"
#include <stdio.h>

int	init(char	**args, t_menu *menu)
{
	t_fork	*forks;

	if (parse(args) == 1)
		return (1);
	//menu = calloc(1, sizeof(t_menu));
	//if (!menu)
	//	return (1);
	menu->no_phls = ft_atoi(args[NO_PHIL]);
	if (menu->no_phls > INT_MAX)
		return (1);
	menu->ttd = ft_atoi(args[TT_DIE]);
	if (menu->ttd > INT_MAX)
		return (1); 
	menu->tte = ft_atoi(args[TT_EAT]);
	if (menu->tte > INT_MAX)
		return (1);
	menu->tts = ft_atoi(args[TT_SLP]);
	menu->meals = ft_atoi(args[NO_MEALS]);
	if (menu->tts > INT_MAX)
		return (1);
	pthread_mutex_init(&menu->master_lock, NULL);
	pthread_mutex_init(&menu->report_lock, NULL);
	pthread_mutex_init(&menu->fat_lock, NULL);
	pthread_mutex_init(&menu->term_lock, NULL);
	menu->philos = malloc(menu->no_phls * sizeof(t_philos));
	if (!menu->philos)
		return (1);
	menu->terminate = false;
	menu->start = get_time(menu);
	return (0);
}

int	main(int    ac, char  **av)
{
	t_menu	menu;

	if (ac != 6)
		return(1);
	if(init(av, &menu) == 1)
		return (0);
	if (prepare(&menu) == 1)
		return (0);
	create_phils(&menu);
	observe(&menu);
	join_threads(&menu);
}


// use pthread_mutex_lock/unlock on every fork so only 1 thread can access each fork