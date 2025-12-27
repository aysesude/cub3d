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

#include "../include/cub3d_bonus.h"

/*
** is_wall - Verilen koordinatin duvar olup olmadigini kontrol eder
** @game: Oyun yapisi
** @x, @y: Kontrol edilecek dünya koordinatlari
**
** Koordinatlari harita indekslerine çevirir (int cast)
** Sinir kontrolleri yapar (negatif veya harita disinda)
**
** Döner:
** - 1: Duvar ('1') veya kapali kapi ('D')
** - 1: Harita disi koordinat
** - 0: Yürünebilir alan ('0', 'o' açik kapi)
*/
/*
** is_wall - Verilen koordinatın duvar olup olmadığını kontrol eder
** @game: Oyun yapısı
** @x, @y: Kontrol edilecek dünya koordinatları
**
** Koordinatları harita indekslerine çevirir (int cast)
** Sınır kontrolleri yapar (negatif veya harita dışında)
**
** Döner:
** - 1: Duvar ('1') veya kapalı kapı ('D')
** - 1: Harita dışı koordinat
** - 0: Yürünebilir alan ('0', 'o' açık kapı)
*/
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

/*
** can_move - Oyuncunun verilen konuma çarpışmadan hareket edip edemeyeceğini kontrol eder
** @game: Oyun yapısı
** @x, @y: Hedef pozisyon
**
** Oyuncunun köşe noktalarını COLLISION_DIST kadar kaydırarak çarpışma kontrolü yapar.
**
** Döner: 1 ise hareket edilebilir, 0 ise engel var
*/
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

/*
** move_forward - Oyuncuyu ileri hareket ettirir
** @game: Oyun yapısı
**
** Oyuncunun yön vektörüne göre ileriye MOVE_SPEED kadar hareket eder.
** Çarpışma kontrolü yapar.
*/
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

/*
** move_backward - Oyuncuyu geri hareket ettirir
** @game: Oyun yapısı
**
** Oyuncunun yön vektörüne göre geriye MOVE_SPEED kadar hareket eder.
** Çarpışma kontrolü yapar.
*/
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

/*
** move_right - Oyuncuyu sağa (strafe) hareket ettirir
** @game: Oyun yapısı
**
** Oyuncunun yönüne dik olarak sağa MOVE_SPEED kadar hareket eder.
** Çarpışma kontrolü yapar.
*/
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
