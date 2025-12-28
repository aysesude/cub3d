/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 03:11:48 by aycami            #+#    #+#             */
/*   Updated: 2025/12/28 12:09:07 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

static void	init_minimap(t_game *game, t_minimap *mm)
{
	int	half;
	int	lp[2];

	mm->tile_size = MINIMAP_TILE;
	half = MINIMAP_VIEW / 2;
	mm->start_x = (int)game->player->x - half;
	mm->start_y = (int)game->player->y - half;
	mm->offset_x = 10;
	mm->offset_y = WIN_HEIGHT - (MINIMAP_VIEW * mm->tile_size) - 10;
	lp[0] = (int)((game->player->x - mm->start_x) * mm->tile_size);
	lp[1] = (int)((game->player->y - mm->start_y) * mm->tile_size);
	mm->px = mm->offset_x + lp[0];
	mm->py = mm->offset_y + lp[1];
}

static void	draw_minimap_bg(t_game *game, t_minimap *mm)
{
	int	yy;
	int	xx;
	int	size;

	size = MINIMAP_VIEW * mm->tile_size;
	yy = 0;
	while (yy < size)
	{
		xx = 0;
		while (xx < size)
		{
			put_pixel(game, mm->offset_x + xx, mm->offset_y + yy, COLOR_BG);
			xx++;
		}
		yy++;
	}
}

static void	draw_tile(t_game *game, t_minimap *mm, int *v)
{
	int		pos[2];
	char	c;

	pos[0] = mm->offset_x + v[0] * mm->tile_size;
	pos[1] = mm->offset_y + v[1] * mm->tile_size;
	c = game->map->grid[mm->start_y + v[1]][mm->start_x + v[0]];
	if (c == '1')
		draw_square(game, pos, mm->tile_size, COLOR_WALL);
	else if (c == 'D')
		draw_square(game, pos, mm->tile_size, COLOR_DOOR);
	else if (c == 'o')
		draw_square(game, pos, mm->tile_size, COLOR_DOOR_OPEN);
}

static void	draw_minimap_tiles(t_game *game, t_minimap *mm)
{
	int	v[2];
	int	m[2];

	v[1] = 0;
	while (v[1] < MINIMAP_VIEW)
	{
		m[1] = mm->start_y + v[1];
		if (m[1] >= 0 && m[1] < game->map->height)
		{
			v[0] = 0;
			while (v[0] < MINIMAP_VIEW)
			{
				m[0] = mm->start_x + v[0];
				if (m[0] >= 0 && m[0] < (int)ft_strlen(game->map->grid[m[1]]))
					draw_tile(game, mm, v);
				v[0]++;
			}
		}
		v[1]++;
	}
}

void	render_mini_map(t_game *game)
{
	t_minimap	mm;
	int			center[2];

	if (game->map->height <= 0)
		return ;
	init_minimap(game, &mm);
	draw_minimap_bg(game, &mm);
	draw_minimap_tiles(game, &mm);
	center[0] = mm.px;
	center[1] = mm.py;
	draw_circle(game, center, mm.tile_size / 3, COLOR_PLAYER);
	draw_minimap_rays(game, &mm);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}
