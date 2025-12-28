/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/28 14:30:47 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/cub3d_bonus.h"

static int	init_and_validate(t_game *game, char *filename)
{
	if (parse_map(game, filename) == -1)
	{
		get_next_line(-1);
		return (-1);
	}
	if (validate_map(game) == -1)
	{
		get_next_line(-1);
		return (-1);
	}
	if (init_graphics(game) == -1)
	{
		get_next_line(-1);
		return (-1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	*game;

	if (argc != 2)
	{
		ft_putstr_fd("Error\nUsage: ./cub3D <map.cub>\n", 2);
		return (1);
	}
	game = init_game();
	if (!game)
	{
		ft_putstr_fd("Error\nMemory allocation failed\n", 2);
		return (1);
	}
	if (init_and_validate(game, argv[1]) == -1)
	{
		cleanup_game(game);
		return (1);
	}
	game_loop_bonus(game);
	cleanup_game(game);
	return (0);
}
