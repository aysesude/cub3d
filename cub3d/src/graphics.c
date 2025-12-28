/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 03:11:50 by aycami            #+#    #+#             */
/*   Updated: 2025/12/28 14:31:41 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	close_window(t_game *game)
{
	cleanup_game(game);
	exit(0);
	return (0);
}

int	init_graphics(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (ft_putstr_fd("Error\nMLX initialization failed\n", 2), -1);
	game->win = mlx_new_window(game->mlx, WIN_WIDTH, WIN_HEIGHT, "cub3D");
	if (!game->win)
		return (ft_putstr_fd("Error\nWindow creation failed\n", 2), -1);
	game->img = mlx_new_image(game->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!game->img)
		return (ft_putstr_fd("Error\nImage creation failed\n", 2), -1);
	game->addr = mlx_get_data_addr(game->img, &game->bits_per_pixel,
			&game->line_length, &game->endian);
	if (!load_textures(game))
		return (ft_putstr_fd("Error\nTexture loading failed\n", 2), -1);
	mlx_hook(game->win, 2, 1L << 0, key_press, game);
	mlx_hook(game->win, 3, 1L << 1, key_release, game);
	mlx_hook(game->win, 17, 0, close_window, game);
	return (0);
}

static int	render_frame(t_game *game)
{
	handle_movement(game);
	render_3d(game);
	return (0);
}

void	game_loop(t_game *game)
{
	mlx_loop_hook(game->mlx, render_frame, game);
	mlx_loop(game->mlx);
}
