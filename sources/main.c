/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/07 15:48:44 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/03/07 16:53:56 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"
#include <stdio.h>

int	main(int ac, char **av)
{
	t_menu	menu;

	if (ac != 6)
		return (1);
	if (parse_args(av, &menu) == 1)
		return (0);
	if (prepare(&menu) == 1)
		return (0);
	create_phils(&menu);
	observe(&menu);
	join_threads(&menu);
}

//check if its lower than 1 in parse