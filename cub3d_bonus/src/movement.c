/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 17:00:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/14 17:14:53 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include <math.h>

#define MOVE_SPEED 0.15
#define ROT_SPEED 0.13962634
#define COLLISION_DIST 0.2

static int	is_wall(t_game *game, double x, double y)
{
	int	map_x;
	int	map_y;

	map_x = (int)x;
	map_y = (int)y;
	if (map_x < 0 || map_y < 0 || map_y >= game->map->height)
		return (1);
	if (map_x >= (int)ft_strlen(game->map->grid[map_y]))
		return (1);
	return (game->map->grid[map_y][map_x] == '1'
		|| game->map->grid[map_y][map_x] == 'D');
}

int	can_move(t_game *game, double x, double y)
{
	if (is_wall(game, x - COLLISION_DIST, y - COLLISION_DIST))
		return (0);
	if (is_wall(game, x + COLLISION_DIST, y - COLLISION_DIST))
		return (0);
	if (is_wall(game, x - COLLISION_DIST, y + COLLISION_DIST))
		return (0);
	if (is_wall(game, x + COLLISION_DIST, y + COLLISION_DIST))
		return (0);
	return (1);
}

void	move_forward(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->x + game->player->dir_x * MOVE_SPEED;
	new_y = game->player->y + game->player->dir_y * MOVE_SPEED;
	if (can_move(game, new_x, game->player->y))
		game->player->x = new_x;
	if (can_move(game, game->player->x, new_y))
		game->player->y = new_y;
}

void	move_backward(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->x - game->player->dir_x * MOVE_SPEED;
	new_y = game->player->y - game->player->dir_y * MOVE_SPEED;
	if (can_move(game, new_x, game->player->y))
		game->player->x = new_x;
	if (can_move(game, game->player->x, new_y))
		game->player->y = new_y;
}

void	move_right(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->x - game->player->dir_y * MOVE_SPEED;
	new_y = game->player->y + game->player->dir_x * MOVE_SPEED;
	if (can_move(game, new_x, game->player->y))
		game->player->x = new_x;
	if (can_move(game, game->player->x, new_y))
		game->player->y = new_y;
}
