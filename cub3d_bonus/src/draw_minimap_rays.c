/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap_rays.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

#define COLOR_RAY 0xAAAAAA
#define COLOR_HIT 0xFF0000

static void	cast_single_ray(t_game *game, double *rd, double *hit)
{
	double	side[5];
	double	delta[2];
	int		data[3];

	data[1] = (int)game->player->x;
	data[2] = (int)game->player->y;
	side[2] = rd[0];
	side[3] = rd[1];
	init_ray_dda(game, rd, side, delta);
	data[0] = 0;
	while (data[0] == 0)
	{
		side[4] = ray_dda_step(&data[1], side, delta);
		data[0] = check_ray_hit(game, &data[1]);
	}
	if (data[0] == 1)
		calc_hit_point(game, rd, hit, side);
	else
	{
		hit[0] = game->player->x + rd[0] * 16.0;
		hit[1] = game->player->y + rd[1] * 16.0;
		hit[2] = 0;
	}
}

static void	clamp_to_minimap(t_minimap *mm, int *p)
{
	int	max_x;
	int	max_y;

	max_x = mm->offset_x + MINIMAP_VIEW * mm->tile_size - 1;
	max_y = mm->offset_y + MINIMAP_VIEW * mm->tile_size - 1;
	if (p[0] < mm->offset_x)
		p[0] = mm->offset_x;
	if (p[0] > max_x)
		p[0] = max_x;
	if (p[1] < mm->offset_y)
		p[1] = mm->offset_y;
	if (p[1] > max_y)
		p[1] = max_y;
}

static int	is_inside_minimap(t_minimap *mm, int *p)
{
	int	max_x;
	int	max_y;

	max_x = mm->offset_x + MINIMAP_VIEW * mm->tile_size - 1;
	max_y = mm->offset_y + MINIMAP_VIEW * mm->tile_size - 1;
	if (p[0] <= mm->offset_x || p[0] >= max_x)
		return (0);
	if (p[1] <= mm->offset_y || p[1] >= max_y)
		return (0);
	return (1);
}

static void	draw_single_ray(t_game *game, t_minimap *mm, double angle)
{
	double	rd[2];
	double	hit[3];
	int		p0[2];
	int		p1[2];

	rd[0] = cos(angle);
	rd[1] = sin(angle);
	cast_single_ray(game, rd, hit);
	p0[0] = mm->px;
	p0[1] = mm->py;
	p1[0] = mm->offset_x + (int)((hit[0] - mm->start_x) * mm->tile_size);
	p1[1] = mm->offset_y + (int)((hit[1] - mm->start_y) * mm->tile_size);
	if ((int)hit[2] && is_inside_minimap(mm, p1))
		draw_circle(game, p1, mm->tile_size / 6, COLOR_HIT);
	clamp_to_minimap(mm, p1);
	draw_line(game, p0, p1, COLOR_RAY);
}

void	draw_minimap_rays(t_game *game, t_minimap *mm)
{
	double	base;
	double	a;

	base = atan2(game->player->dir_y, game->player->dir_x);
	a = -33.0;
	while (a <= 33.0001)
	{
		draw_single_ray(game, mm, base + deg_to_rad(a));
		a += 6.0;
	}
}
