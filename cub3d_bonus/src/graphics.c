/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 03:11:50 by aycami            #+#    #+#             */
/*   Updated: 2025/12/14 17:03:59 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

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

static void	check_door_dist(t_game *game, int x, int y, int *p)
{
	double	c[2];
	double	d[2];
	double	dist;

	if (game->map->grid[y][x] != 'D' && game->map->grid[y][x] != 'o')
		return ;
	c[0] = x + 0.5;
	c[1] = y + 0.5;
	d[0] = game->player->x - c[0];
	d[1] = game->player->y - c[1];
	dist = sqrt(d[0] * d[0] + d[1] * d[1]);
	if (dist <= 2.0 && game->map->grid[y][x] == 'D')
		game->map->grid[y][x] = 'o';
	else if (dist > 2.0 && game->map->grid[y][x] == 'o')
	{
		if (!(p[0] == x && p[1] == y))
			game->map->grid[y][x] = 'D';
	}
}

static void	handle_doors(t_game *game)
{
	int	p[2];
	int	y;
	int	x;

	p[0] = (int)game->player->x;
	p[1] = (int)game->player->y;
	y = p[1] - 2;
	while (y <= p[1] + 2)
	{
		if (y >= 0 && y < game->map->height)
		{
			x = p[0] - 2;
			while (x <= p[0] + 2)
			{
				if (x >= 0 && x < (int)ft_strlen(game->map->grid[y]))
					check_door_dist(game, x, y, p);
				x++;
			}
		}
		y++;
	}
}

static int	render_frame(t_game *game)
{
	handle_movement(game);
	handle_doors(game);
	render_3d(game);
	render_mini_map(game);
	return (0);
}

void	game_loop(t_game *game)
{
	mlx_loop_hook(game->mlx, render_frame, game);
	mlx_loop(game->mlx);
}
