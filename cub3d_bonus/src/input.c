/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 17:10:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/14 17:02:13 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

int	key_press(int keycode, t_game *game)
{
	if (keycode == 65307)
	{
		cleanup_game(game);
		exit(0);
	}
	if (keycode == 119)
		game->keys.w = 1;
	if (keycode == 115)
		game->keys.s = 1;
	if (keycode == 97)
		game->keys.a = 1;
	if (keycode == 100)
		game->keys.d = 1;
	if (keycode == 65363)
		game->keys.right = 1;
	if (keycode == 65361)
		game->keys.left = 1;
	return (0);
}

int	key_release(int keycode, t_game *game)
{
	if (keycode == 119)
		game->keys.w = 0;
	if (keycode == 115)
		game->keys.s = 0;
	if (keycode == 97)
		game->keys.a = 0;
	if (keycode == 100)
		game->keys.d = 0;
	if (keycode == 65363)
		game->keys.right = 0;
	if (keycode == 65361)
		game->keys.left = 0;
	return (0);
}

void	handle_movement(t_game *game)
{
	if (game->keys.w)
		move_forward(game);
	if (game->keys.s)
		move_backward(game);
	if (game->keys.a)
		move_left(game);
	if (game->keys.d)
		move_right(game);
	if (game->keys.left)
		rotate_right(game);
	if (game->keys.right)
		rotate_left(game);
}
