#include "philosophers.h"

int main(int    ac, char  **av)
{
	if (ac != 5)
		return(1);
	if(!(parse_args(av)))
		return (1);
	


}


// use pthread_mutex_lock/unlock on every fork so only 1 thread can access each fork