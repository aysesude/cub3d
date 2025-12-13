/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 03:11:48 by aycami            #+#    #+#             */
/*   Updated: 2025/12/07 11:37:26 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

#define COLOR_WALL 0x555555
#define COLOR_DOOR 0x996633
#define COLOR_DOOR_OPEN 0xCC9966
#define COLOR_PLAYER 0x00FF00
#define COLOR_BG 0x222222
#define COLOR_RAY 0xAAAAAA
#define COLOR_HIT 0xFF0000

void put_pixel(t_game *game, int x, int y, int color)
{
	char *dst;
	if (x < 0 || y < 0 || x >= WIN_WIDTH || y >= WIN_HEIGHT)
		return;
	dst = game->addr + (y * game->line_length + x * (game->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void draw_square(t_game *game, int x, int y, int size, int color)
{
	int i = 0;
	while (i < size)
	{
		int j = 0;
		while (j < size)
		{
			put_pixel(game, x + j, y + i, color);
			j++;
		}
		i++;
	}
}

void draw_circle(t_game *game, int cx, int cy, int r, int color)
{
	int y = -r;
	while (y <= r)
	{
		int x = -r;
		while (x <= r)
		{
			if (x * x + y * y <= r * r)
				put_pixel(game, cx + x, cy + y, color);
			x++;
		}
		y++;
	}
}

void draw_line(t_game *game, int x0, int y0, int x1, int y1, int color)
{
	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;
	while (1)
	{
		put_pixel(game, x0, y0, color);
		if (x0 == x1 && y0 == y1) break;
		int e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

double deg_to_rad(double deg)
{
	return deg * (M_PI / 180.0);
}

static int get_map_max_width(t_map *map)
{
	int y = 0;
	int maxw = 0;
	while (y < map->height)
	{
		int w = (int)ft_strlen(map->grid[y]);
		if (w > maxw)
			maxw = w;
		y++;
	}
	return maxw;
}

void render_mini_map(t_game *game)
{
	int map_h = game->map->height;
	int map_w = get_map_max_width(game->map);
	if (map_w <= 0 || map_h <= 0)
		return;

	// Görüntülenecek pencere: oyuncu çevresinde 12x12 tile
	int view_size = 12;
	int half = view_size / 2; // 6
	int pxi = (int)game->player->x;
	int pyi = (int)game->player->y;
	int start_x = pxi - half;
	int start_y = pyi - half;
	(void)map_w; // kullanılmıyor uyarısını önlemek için
	// end_x/end_y şimdilik kullanılmıyor; görünüm boyutu sabit

	// Minimap kutu boyutu
	int max_px_w = 256;
	int max_px_h = 256;
	int tile_size_w = max_px_w / view_size;
	int tile_size_h = max_px_h / view_size;
	int tile_size = tile_size_w < tile_size_h ? tile_size_w : tile_size_h;
	if (tile_size < 2)
		tile_size = 2;

	int map_px_w = view_size * tile_size;
	int map_px_h = view_size * tile_size;

	int offset_x = 10; // sol alt köşe
	int offset_y = WIN_HEIGHT - map_px_h - 10;

	// Arka plan
	for (int yy = 0; yy < map_px_h; yy++)
		for (int xx = 0; xx < map_px_w; xx++)
			put_pixel(game, offset_x + xx, offset_y + yy, COLOR_BG);

	// Pencere içeriğini çiz
	for (int vy = 0; vy < view_size; vy++)
	{
		int my = start_y + vy;
		if (my < 0 || my >= map_h)
			continue;
		int rowlen = (int)ft_strlen(game->map->grid[my]);
		for (int vx = 0; vx < view_size; vx++)
		{
			int mx = start_x + vx;
			if (mx < 0 || mx >= rowlen)
				continue;
			int sx = offset_x + vx * tile_size;
			int sy = offset_y + vy * tile_size;
			char c = game->map->grid[my][mx];
			if (c == '1')
				draw_square(game, sx, sy, tile_size, COLOR_WALL);
			else if (c == 'D')
				draw_square(game, sx, sy, tile_size, COLOR_DOOR);
			else if (c == 'o')
				draw_square(game, sx, sy, tile_size, COLOR_DOOR_OPEN);
		}
	}

	// Oyuncu yerel koordinatları
	int lp_x = (int)((game->player->x - start_x) * tile_size);
	int lp_y = (int)((game->player->y - start_y) * tile_size);
	int px = offset_x + lp_x;
	int py = offset_y + lp_y;
	draw_circle(game, px, py, tile_size / 3, COLOR_PLAYER);

	// Işınlar: FOV içinde rayları at ve duvara/kapıya çarpana kadar çiz
	double fov = 66.0;
	double half_fov = fov / 2.0;
	double step_deg = 6.0; // daha sık ışın
	double base_angle = atan2(game->player->dir_y, game->player->dir_x);
	for (double a = -half_fov; a <= half_fov + 0.0001; a += step_deg)
	{
		double ray_angle = base_angle + deg_to_rad(a);
		double rdx = cos(ray_angle);
		double rdy = sin(ray_angle);

		int mapX = (int)game->player->x;
		int mapY = (int)game->player->y;
		double sideDistX, sideDistY;
		double deltaDistX = (rdx == 0) ? 1e30 : fabs(1.0 / rdx);
		double deltaDistY = (rdy == 0) ? 1e30 : fabs(1.0 / rdy);
		int stepX, stepY;
		int hit = 0;
		int side = 0;

		if (rdx < 0) { stepX = -1; sideDistX = (game->player->x - mapX) * deltaDistX; }
		else { stepX = 1; sideDistX = (mapX + 1.0 - game->player->x) * deltaDistX; }
		if (rdy < 0) { stepY = -1; sideDistY = (game->player->y - mapY) * deltaDistY; }
		else { stepY = 1; sideDistY = (mapY + 1.0 - game->player->y) * deltaDistY; }

		int guard = 0;
		while (!hit && guard < 512)
		{
			if (sideDistX < sideDistY)
			{ sideDistX += deltaDistX; mapX += stepX; side = 0; }
			else
			{ sideDistY += deltaDistY; mapY += stepY; side = 1; }

			if (mapY < 0 || mapY >= map_h)
				break;
			if (mapX < 0 || mapX >= (int)ft_strlen(game->map->grid[mapY]))
				break;
			char cell = game->map->grid[mapY][mapX];
			if (cell == '1' || cell == 'D')
			{ hit = 1; break; }
			guard++;
		}

		double hitX, hitY;
		if (hit)
		{
			double perp;
			if (side == 0) perp = (mapX - game->player->x + (1 - stepX) / 2.0) / rdx;
			else perp = (mapY - game->player->y + (1 - stepY) / 2.0) / rdy;
			hitX = game->player->x + perp * rdx;
			hitY = game->player->y + perp * rdy;
		}
		else
		{
			double maxd = 16.0; // minimap için kısa menzil
			hitX = game->player->x + rdx * maxd;
			hitY = game->player->y + rdy * maxd;
		}

		int hx_local = (int)((hitX - start_x) * tile_size);
		int hy_local = (int)((hitY - start_y) * tile_size);
		int hx = offset_x + hx_local;
		int hy = offset_y + hy_local;
		draw_line(game, px, py, hx, hy, COLOR_RAY);
		if (hit) draw_circle(game, hx, hy, tile_size / 6, COLOR_HIT);
	}

	// Son görüntüyü ekrana bas
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}
