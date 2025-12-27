/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

/*
** put_pixel - Ekrandaki bir pikseli verilen renkle boyar
** @game: Oyun yapısı
** @x, @y: Piksel koordinatları
** @color: Renk (hex)
**
** MLX image buffer'ına doğrudan erişerek (addr) pikseli renklendirir.
*/
void	put_pixel(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || y < 0 || x >= WIN_WIDTH || y >= WIN_HEIGHT)
		return ;
	dst = game->addr + (y * game->line_length + x * (game->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

/*
** draw_square - Minimap'ta kare bir alanı boyar
** @game: Oyun yapısı
** @pos: Sol üst köşe koordinatı (x, y)
** @size: Karenin kenar uzunluğu (px)
** @color: Renk (hex)
*/
void	draw_square(t_game *game, int *pos, int size, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			put_pixel(game, pos[0] + j, pos[1] + i, color);
			j++;
		}
		i++;
	}
}

/*
** draw_circle - Minimap'ta daire çizer (ör. oyuncu veya ray vuruş noktası)
** @game: Oyun yapısı
** @center: Merkez koordinatı (x, y)
** @r: Yarıçap (px)
** @color: Renk (hex)
*/
void	draw_circle(t_game *game, int *center, int r, int color)
{
	int	y;
	int	x;

	y = -r;
	while (y <= r)
	{
		x = -r;
		while (x <= r)
		{
			if (x * x + y * y <= r * r)
				put_pixel(game, center[0] + x, center[1] + y, color);
			x++;
		}
		y++;
	}
}

/*
** draw_line_step - Bresenham algoritmasında bir adım atar
** @pos: Mevcut piksel koordinatı (x, y)
** @delta: Farklar ve yönler
** @err: Hata değeri
*/
static void	draw_line_step(int *pos, int *delta, int *err)
{
	int	e2;

	e2 = 2 * err[0];
	if (e2 >= delta[1])
	{
		err[0] += delta[1];
		pos[0] += delta[2];
	}
	if (e2 <= delta[0])
	{
		err[0] += delta[0];
		pos[1] += delta[3];
	}
}

/*
** draw_line - İki nokta arasında doğru çizer (Bresenham algoritması)
** @game: Oyun yapısı
** @p0: Başlangıç noktası (x, y)
** @p1: Bitiş noktası (x, y)
** @color: Renk (hex)
*/
void	draw_line(t_game *game, int *p0, int *p1, int color)
{
	int	delta[4];
	int	pos[2];
	int	err[1];

	delta[0] = abs(p1[0] - p0[0]);
	delta[1] = -abs(p1[1] - p0[1]);
	delta[2] = -1 + (2 * (p0[0] < p1[0]));
	delta[3] = -1 + (2 * (p0[1] < p1[1]));
	err[0] = delta[0] + delta[1];
	pos[0] = p0[0];
	pos[1] = p0[1];
	while (1)
	{
		put_pixel(game, pos[0], pos[1], color);
		if (pos[0] == p1[0] && pos[1] == p1[1])
			break ;
		draw_line_step(pos, delta, err);
	}
}
