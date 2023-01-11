#include "philosophers.h"
#include <stdbool.h>

int	ft_atoi(const char	*s)
{
	int				i;
	unsigned long	num;

	i = 1;
	num = 0;
	while (*s == '\n' || *s == '\t' || *s == '\r' || *s == '\n'
		|| *s == '\v' || *s == '\f' || *s == ' ')
		s++;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			i = -1;
		s++;
	}
	while ((*s >= '0') && (*s <= '9'))
	{
		num = (num * 10) + (*s - '0');
		s++;
	}
	if (num > 9223372036854775807)
		return (0);
	return ((int)num * i);
}

void	prep(t_philos *philo)
{
	t_menu	*menu;
	
	menu = philo->menu;
	philo->existence = true;
	printf("loser number %i has awoken\n", philo->id);
}

void	*birth(void	*param)
{
	t_philos	*philo;
	t_menu		*menu;

	philo = param;
	menu = philo->menu;
	prep(philo);
	pthread_mutex_lock(philo->right);
	printf("loser number %i is eating\n", philo->id);
	// pthread_mutex_lock(philo->left);
// 	printf("loser number %i is eating\n", philo->id);
// 	usleep(500);
	pthread_mutex_unlock(philo->right);
// 	pthread_mutex_unlock(philo->left);
}

void	create_phils(t_menu	*menu)
{
	int	i;

	i = 0;
	while(++i < menu->no_phls)
		pthread_create(&menu->philos[i].thread, NULL, &birth, &menu->philos[i]);
	i = -1;
	while (++i < menu->no_phls)
		pthread_join(menu->philos[i].thread, NULL);
}

int	prepare(t_menu	*menu)
{
	int	i;

	i = -1;
	while (++i < menu->no_phls)
	{
		menu->philos[i].id = i + 1;
		menu->philos[i].right = malloc(1 * sizeof(pthread_mutex_t));
		if (pthread_mutex_init(menu->philos[i].right, NULL)!= 0)
			return (1);
		if (i != 0)
			menu->philos[i].left = menu->philos[i - 1].right;
	}
	if (i > 0)
		menu->philos[0].left = menu->philos[menu->no_phls].right;
	else
		menu->philos[0].left = NULL
	return (0);
}

/*still needs proper error handling or parsing
needs time of day*/
int	parse_args(char	**args)
{
	t_menu	*menu;
	t_fork	*forks;

	menu = calloc(1, sizeof(t_menu));
	menu->no_phls = ft_atoi(args[NO_PHIL]);
	menu->ttd = ft_atoi(args[TT_DIE]);
	menu->tte = ft_atoi(args[TT_EAT]);
	menu->tts = ft_atoi(args[TT_SLP]);
	menu->philos = malloc(menu->no_phls * sizeof(t_philos));
	// menu->clock = gettimeofday()
	prepare(menu);
	return (0);
}