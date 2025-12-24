/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 19:51:54 by raktas            #+#    #+#             */
/*   Updated: 2025/12/24 15:44:17 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/cub3d.h"

static int	validate_arguments(int argc)
{
	if (argc != 2)
	{
		ft_putstr_fd("Error\nUsage: ./cub3D <map.cub>\n", 2);
		return (-1);
	}
	return (0);
}

static int	initialize_game_data(t_game **game)
{
	*game = init_game();
	if (!*game)
	{
		ft_putstr_fd("Error\nMemory allocation failed\n", 2);
		return (-1);
	}
	return (0);
}

static int	load_and_validate_map(t_game *game, char *filename)
{
	if (parse_map(game, filename) == -1)
	{
		get_next_line(-1);
		cleanup_game(game);
		return (-1);
	}
	if (validate_map(game) == -1)
	{
		get_next_line(-1);
		cleanup_game(game);
		return (-1);
	}
	return (0);
}

static int	setup_graphics(t_game *game)
{
	if (init_graphics(game) == -1)
	{
		get_next_line(-1);
		cleanup_game(game);
		return (-1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	*game;

	if (validate_arguments(argc) == -1)
		return (1);
	if (initialize_game_data(&game) == -1)
		return (1);
	if (load_and_validate_map(game, argv[1]) == -1)
		return (1);
	if (setup_graphics(game) == -1)
		return (1);
	game_loop(game);
	cleanup_game(game);
	return (0);
}
