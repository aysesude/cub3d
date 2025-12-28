/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 03:11:53 by aycami            #+#    #+#             */
/*   Updated: 2025/12/28 12:07:15 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

static t_map	*init_map(void)
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		return (NULL);
	map->grid = NULL;
	map->width = 0;
	map->height = 0;
	map->no_texture = NULL;
	map->so_texture = NULL;
	map->we_texture = NULL;
	map->ea_texture = NULL;
	map->do_texture = NULL;
	map->floor_color = -1;
	map->ceiling_color = -1;
	return (map);
}

static t_player	*init_player(void)
{
	t_player	*player;

	player = malloc(sizeof(t_player));
	if (!player)
		return (NULL);
	player->x = 0;
	player->y = 0;
	player->dir_x = -1;
	player->dir_y = 0;
	player->plane_x = 0;
	player->plane_y = 0.65;
	return (player);
}

static void	init_keys(t_game *game)
{
	game->keys.w = 0;
	game->keys.s = 0;
	game->keys.a = 0;
	game->keys.d = 0;
	game->keys.left = 0;
	game->keys.right = 0;
}

static void	init_textures(t_game *game)
{
	game->tex_no.img = NULL;
	game->tex_so.img = NULL;
	game->tex_we.img = NULL;
	game->tex_ea.img = NULL;
	game->tex_do.img = NULL;
}

t_game	*init_game(void)
{
	t_game	*game;

	game = malloc(sizeof(t_game));
	if (!game)
		return (NULL);
	game->mlx = NULL;
	game->win = NULL;
	game->img = NULL;
	game->addr = NULL;
	init_textures(game);
	game->map = init_map();
	if (!game->map)
		return (free(game), NULL);
	game->player = init_player();
	if (!game->player)
		return (free(game->map), free(game), NULL);
	init_keys(game);
	return (game);
}
