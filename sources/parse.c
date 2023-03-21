/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/07 16:45:27 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/03/21 20:14:29 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	print_error(void)
{
	write(1, "invalid arguements: expected numeric input\n", 44);
	return (1);
}

int	parse(char	**av)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	while (av[++i])
	{
		x = 0;
		while (av[i][x])
		{
			if (ft_isdigit(av[i][x]) == false || av[i][x] < 0)
				return (print_error());
			x++;
		}
	}
	return (0);
}

int	parse_args(char	**args, t_menu *menu)
{
	if (parse(args) == 1)
		return (1);
	menu->no_phls = ft_atoi(args[NO_PHIL]);
	if (menu->no_phls > INT_MAX || menu->no_phls <= 0)
		return (print_error());
	menu->ttd = ft_atoi(args[TT_DIE]);
	if (menu->ttd > INT_MAX || menu->ttd <= 0)
		return (print_error());
	menu->tte = ft_atoi(args[TT_EAT]);
	if (menu->tte > INT_MAX || menu->tte <= 0)
		return (print_error());
	menu->tts = ft_atoi(args[TT_SLP]);
	if (menu->tts > INT_MAX || menu->tts <= 0)
		return (print_error());
	menu->meals = ft_atoi(args[NO_MEALS]);
	if (menu->meals > INT_MAX || menu->meals < 0)
		return (print_error());
	if (menu->tts > INT_MAX || menu->tts <= 0)
		return (print_error());
	menu->terminate = false;
	menu->start = get_time(menu);
	return (init_mutex(menu));
}
