/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

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

void	handle_doors(t_game *game)
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
