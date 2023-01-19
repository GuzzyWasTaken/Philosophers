#include "philosophers.h"
#include <stdio.h>

int	main(int    ac, char  **av)
{

	int		i;

	if (ac != 6)
		return(1);
	init(av);
	parse_args(av);
	// menu->s_args->phils = ft_atoi(av[1]);
	// create_t(menu);
	// join_t(menu);
}


// use pthread_mutex_lock/unlock on every fork so only 1 thread can access each fork