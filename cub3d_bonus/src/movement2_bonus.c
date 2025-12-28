/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement2_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 17:00:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/28 12:05:07 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

void	move_left(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->x + game->player->dir_y * MOVE_SPEED;
	new_y = game->player->y - game->player->dir_x * MOVE_SPEED;
	if (can_move(game, new_x, game->player->y))
		game->player->x = new_x;
	if (can_move(game, game->player->x, new_y))
		game->player->y = new_y;
}

void	rotate_left(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player->dir_x;
	old_plane_x = game->player->plane_x;
	game->player->dir_x = game->player->dir_x * cos(ROT_SPEED)
		- game->player->dir_y * sin(ROT_SPEED);
	game->player->dir_y = old_dir_x * sin(ROT_SPEED)
		+ game->player->dir_y * cos(ROT_SPEED);
	game->player->plane_x = game->player->plane_x * cos(ROT_SPEED)
		- game->player->plane_y * sin(ROT_SPEED);
	game->player->plane_y = old_plane_x * sin(ROT_SPEED)
		+ game->player->plane_y * cos(ROT_SPEED);
}

void	rotate_right(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player->dir_x;
	old_plane_x = game->player->plane_x;
	game->player->dir_x = game->player->dir_x * cos(-ROT_SPEED)
		- game->player->dir_y * sin(-ROT_SPEED);
	game->player->dir_y = old_dir_x * sin(-ROT_SPEED)
		+ game->player->dir_y * cos(-ROT_SPEED);
	game->player->plane_x = game->player->plane_x * cos(-ROT_SPEED)
		- game->player->plane_y * sin(-ROT_SPEED);
	game->player->plane_y = old_plane_x * sin(-ROT_SPEED)
		+ game->player->plane_y * cos(-ROT_SPEED);
}
