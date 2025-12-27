/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 03:11:53 by aycami            #+#    #+#             */
/*   Updated: 2025/12/14 16:50:11 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

/*
** init_map - Harita yapisini olusturur ve sifirlar
**
** Yeni bir t_map yapisi ayirir ve tüm alanlarini
** varsayilan degerlerle baslatir:
** - grid: NULL (harita dizisi)
** - width/height: 0 (boyutlar)
** - texture yollari: NULL (NO, SO, WE, EA, DO)
** - floor_color/ceiling_color: -1 (henüz ayarlanmadi)
**
** Basarili ise t_map pointer, hata durumunda NULL döner
*/
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

/*
** init_player - Oyuncu yapisini olusturur ve sifirlar
**
** Yeni bir t_player yapisi ayirir ve varsayilan degerlerle baslatir:
** - x, y: 0 (pozisyon, daha sonra haritadan ayarlanir)
** - dir_x: -1, dir_y: 0 (varsayilan bakis yönü: Kuzey)
** - plane_x: 0, plane_y: 0.65 (kamera düzlemi, FOV için)
**
** Kamera düzlemi bakis vektörüne diktir ve
** görüs alanini (FOV ~66 derece) belirler
**
** Basarili ise t_player pointer, hata durumunda NULL döner
*/
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

/*
** init_keys - Klavye durumunu sifirlar
** @game: Oyun yapisi
**
** Tüm hareket tuslarinin durumunu 0 (basilmadi) olarak ayarlar:
** - w: ileri, s: geri
** - a: sol strafe, d: sag strafe
** - left/right: saga/sola dönüs
**
** Bu degerler key_press/key_release ile güncellenir
*/
static void	init_keys(t_game *game)
{
	game->keys.w = 0;
	game->keys.s = 0;
	game->keys.a = 0;
	game->keys.d = 0;
	game->keys.left = 0;
	game->keys.right = 0;
}

/*
** init_textures - Texture yapilarini sifirlar
** @game: Oyun yapisi
**
** Tüm texture img pointerlarini NULL olarak ayarlar:
** - tex_no: Kuzey duvari textureü
** - tex_so: Güney duvari textureü
** - tex_we: Bati duvari textureü
** - tex_ea: Dogu duvari textureü
** - tex_do: Kapi textureü (bonus)
**
** Gerçek textureler load_textures ile yüklenir
*/
static void	init_textures(t_game *game)
{
	game->tex_no.img = NULL;
	game->tex_so.img = NULL;
	game->tex_we.img = NULL;
	game->tex_ea.img = NULL;
	game->tex_do.img = NULL;
}

/*
** init_game - Ana oyun yapisini olusturur ve baslatir
**
** Tüm oyun bilesenlerini ayirir ve baslatir:
** 1. t_game yapisi için bellek ayir
** 2. MLX pointerlarini NULL yap (mlx, win, img, addr)
** 3. init_textures ile texture yapilarini sifirla
** 4. init_map ile harita yapisini olustur
** 5. init_player ile oyuncu yapisini olustur
** 6. init_keys ile klavye durumunu sifirla
**
** Herhangi bir ayirma basarisiz olursa,
** önceki ayirmalari temizler ve NULL döner
**
** Basarili ise t_game pointer döner
*/
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
