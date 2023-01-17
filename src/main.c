#include "philosophers.h"
#include <stdio.h>

// int	ft_atoi(const char	*s)
// {
// 	int				i;
// 	unsigned long	num;

// 	i = 1;
// 	num = 0;
// 	while (*s == '\n' || *s == '\t' || *s == '\r' || *s == '\n'
// 		|| *s == '\v' || *s == '\f' || *s == ' ')
// 		s++;
// 	if (*s == '+' || *s == '-')
// 	{
// 		if (*s == '-')
// 			i = -1;
// 		s++;
// 	}
// 	while ((*s >= '0') && (*s <= '9'))
// 	{
// 		num = (num * 10) + (*s - '0');
// 		s++;
// 	}
// 	if (num > 9223372036854775807)
// 		return (0);
// 	return ((int)num * i);
// }

// void	*penis(void *param)
// {
// 	int	i;
// 	t_philos *philo;
// 	t_menu *menu;

// 	philo = param;
// 	menu = philo->menu;
// 	i = 0;

// 	// pthread_mutex_lock(menu->philos->right);
// 	// pthread_mutex_lock(menu->philos->left);
// 	while(i < 1000)
// 	{
// 		i++;
// 		printf("loser[%i] is eating..\n", philo->id);
// 		usleep(50000);
// 		printf("loser[%i] is sleeping..\n", philo->id);
// 		usleep(50000);
// 		printf("loser[%i] is thinking..\n", philo->id);
// 	}
// 	// pthread_mutex_unlock(menu->philos->right);
// 	// pthread_mutex_unlock(menu->philos->left);
// 	return(NULL);
// }

// void	join_t(t_menu	*menu)
// {
// 	int	i;

// 	i = 0;
// 	while(i < menu->s_args->phils)
// 	{
// 		pthread_join(menu->philos[i].thread, NULL);
// 		i++;
// 	}
// }

// void	create_t(t_menu	*menu)
// {
// 	int	i;

// 	i = 0;
// 	printf("hello\n");
// 	while(i < menu->s_args->phils)
// 	{
// 		pthread_create(&menu->philos[i].thread, NULL, &penis, &menu->philos[i]);
// 		// pthread_mutex_init ()
// 		menu->philos[i].id = i + 1;
// 		i++;
// 	}
// }

int	main(int    ac, char  **av)
{

	int		i;

	if (ac != 6)
		return(1);
	
	parse_args(av);
	// menu->s_args->phils = ft_atoi(av[1]);
	// create_t(menu);
	// join_t(menu);
	


}


// use pthread_mutex_lock/unlock on every fork so only 1 thread can access each fork