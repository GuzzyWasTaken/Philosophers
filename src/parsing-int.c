#include "philosophers.h"

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

int	fill_phils (t_philo	*philo)
{

}

int	parse_args(char	**args)
{
	int	i;
	int	num_of_ph;
	t_philo	*philo;
	t_fork	*forks;

	num_of_ph = atoi(args[0]);
	philo = malloc(num_of_ph * sizeof(t_philo));
	if (!philo)
		return(1);
	
}