#include "philosophers.h"
#include <stdio.h>

int	main(int    ac, char  **av)
{

	int		i;

	if (ac != 6)
		return(1);
	if(init(av) == 1)
		return (0);

	// menu->s_args->phils = ft_atoi(av[1]);
	// create_t(menu);
	// join_t(menu);
}


// use pthread_mutex_lock/unlock on every fork so only 1 thread can access each fork