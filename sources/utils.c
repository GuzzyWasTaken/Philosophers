/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/07 15:49:05 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/03/14 16:47:42 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

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

unsigned long	get_time(t_menu *menu)
{
	struct timeval	tv;
	unsigned long	curtime;

	gettimeofday(&tv, NULL);
	curtime = (tv.tv_sec * 1000);
	curtime += (tv.tv_usec / 1000);
	return (curtime - menu->start);
}

bool	ft_isdigit(char a)
{
	if ((a >= '0') && (a <= '9'))
		return (true);
	else
		return (false);
}

void	better_sleep(unsigned long duration, t_menu *menu)
{
	unsigned long	goal;
	unsigned long	now;
	unsigned long	delta;

	goal = get_time(menu) + duration;
	while (true)
	{
		now = get_time(menu);
		if (now >= goal)
			return ;
		delta = goal - now;
		if (delta <= 1)
		{
			while (get_time(menu) < goal)
				usleep(200);
			return ;
		}
		usleep((delta * 2) / 3);
	}
}
