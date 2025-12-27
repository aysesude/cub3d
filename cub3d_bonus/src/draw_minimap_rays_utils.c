/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap_rays_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

/*
** init_ray_dda - Minimap için DDA algoritması başlangıç değerlerini ayarlar
** @game: Oyun yapısı
** @rd: Işının yön vektörü (x, y)
** @side: DDA için ilk yan mesafeler ve yönler
** @delta: DDA için x ve y ekseninde bir grid ilerlemek için gereken mesafe
**
** DDA (Digital Differential Analyzer) algoritmasında, ışının gridde hangi
** hücreye ilk önce çarpacağını bulmak için başlangıç mesafeleri ve adımlar hesaplanır.
*/
void	init_ray_dda(t_game *game, double *rd, double *side, double *delta)
{
	int	map[2];

	map[0] = (int)game->player->x;
	map[1] = (int)game->player->y;
	if (rd[0] == 0)
		delta[0] = 1e30;
	else
		delta[0] = fabs(1.0 / rd[0]);
	if (rd[1] == 0)
		delta[1] = 1e30;
	else
		delta[1] = fabs(1.0 / rd[1]);
	if (rd[0] < 0)
		side[0] = (game->player->x - map[0]) * delta[0];
	else
		side[0] = (map[0] + 1.0 - game->player->x) * delta[0];
	if (rd[1] < 0)
		side[1] = (game->player->y - map[1]) * delta[1];
	else
		side[1] = (map[1] + 1.0 - game->player->y) * delta[1];
}

/*
** check_ray_hit - DDA sırasında ışının bir duvara veya kapıya çarpıp çarpmadığını kontrol eder
** @game: Oyun yapısı
** @map: Kontrol edilecek grid koordinatları (x, y)
**
** Döner:
** -1: Harita dışı
**  1: Duvar ('1') veya kapalı kapı ('D')
**  0: Boşluk veya açık kapı
*/
int	check_ray_hit(t_game *game, int *map)
{
	char	cell;

	if (map[1] < 0 || map[1] >= game->map->height)
		return (-1);
	if (map[0] < 0 || map[0] >= (int)ft_strlen(game->map->grid[map[1]]))
		return (-1);
	cell = game->map->grid[map[1]][map[0]];
	if (cell == '1' || cell == 'D')
		return (1);
	return (0);
}

/*
** ray_dda_step - DDA algoritmasında bir adım atar ve hangi eksende ilerlediğini döner
** @map: Mevcut grid koordinatları (x, y)
** @side: DDA için yan mesafeler ve yönler
** @delta: DDA için x ve y ekseninde bir grid ilerlemek için gereken mesafe
**
** Döner:
** 0: X ekseninde adım atıldı
** 1: Y ekseninde adım atıldı
*/
int	ray_dda_step(int *map, double *side, double *delta)
{
	int		step[2];
	int		ret;

	step[0] = -1 + 2 * (side[2] >= 0);
	step[1] = -1 + 2 * (side[3] >= 0);
	if (side[0] < side[1])
	{
		side[0] += delta[0];
		map[0] += step[0];
		ret = 0;
	}
	else
	{
		side[1] += delta[1];
		map[1] += step[1];
		ret = 1;
	}
	return (ret);
}

/*
** deg_to_rad - Dereceyi radyana çevirir
** @deg: Derece cinsinden açı
**
** Döner: Radyan cinsinden açı (double)
*/
double	deg_to_rad(double deg)
{
	return (deg * (M_PI / 180.0));
}

/*
** calc_hit_point - DDA sonunda ışının duvara çarptığı noktayı hesaplar
** @game: Oyun yapısı
** @rd: Işının yön vektörü (x, y)
** @hit: Sonuç olarak çarpılan noktanın koordinatları (x, y, hit flag)
** @side: DDA yan mesafeleri ve hangi eksende çarpıldığı bilgisi
**
** DDA algoritması sonunda, ışının duvara çarptığı gerçek dünya koordinatını hesaplar.
** hit[2] = 1 ise çarpma var, 0 ise yok.
*/
void	calc_hit_point(t_game *game, double *rd, double *hit, double *side)
{
	double	perp;
	double	delta[2];

	delta[0] = 1e30;
	delta[1] = 1e30;
	if (rd[0] != 0)
		delta[0] = fabs(1.0 / rd[0]);
	if (rd[1] != 0)
		delta[1] = fabs(1.0 / rd[1]);
	if (side[4] == 0)
		perp = side[0] - delta[0];
	else
		perp = side[1] - delta[1];
	hit[0] = game->player->x + perp * rd[0];
	hit[1] = game->player->y + perp * rd[1];
	hit[2] = 1;
}
