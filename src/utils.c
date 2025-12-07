/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 03:12:01 by aycami            #+#    #+#             */
/*   Updated: 2025/12/07 03:13:04 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	cleanup_map(t_game *game)
{
	int	i;

	if (!game->map)
		return ;
	if (game->map->grid)
	{
		i = 0;
		while (i < game->map->height)
		{
			free(game->map->grid[i]);
			i++;
		}
		free(game->map->grid);
	}
	free(game->map->no_texture);
	free(game->map->so_texture);
	free(game->map->we_texture);
	free(game->map->ea_texture);
	free(game->map);
}

void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
	cleanup_map(game);
	if (game->player)
		free(game->player);
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
		mlx_destroy_display(game->mlx);
	free(game);
}
