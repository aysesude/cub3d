/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_3d_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 03:11:41 by aycami            #+#    #+#             */
/*   Updated: 2025/12/14 17:42:13 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	calc_tex_x(t_ray *ray, t_texture *tex, t_player *player)
{
	double	wall_x;
	int		tex_x;

	if (ray->side == 0)
		wall_x = player->y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		wall_x = player->x + ray->perp_wall_dist * ray->ray_dir_x;
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * (double)tex->width);
	if ((ray->side == 0 && ray->ray_dir_x < 0)
		|| (ray->side == 1 && ray->ray_dir_y > 0))
		tex_x = tex->width - tex_x - 1;
	return (tex_x);
}

static void	draw_wall_texture(t_game *game, t_ray *ray, int x, t_texture *tex)
{
	int		y;
	int		tex_x;
	int		tex_y;
	double	step;
	double	tex_pos;

	tex_x = calc_tex_x(ray, tex, game->player);
	step = 1.0 * tex->height / ray->line_height;
	tex_pos = (ray->draw_start - WIN_HEIGHT / 2 + ray->line_height / 2) * step;
	y = ray->draw_start;
	while (y <= ray->draw_end)
	{
		tex_y = (int)tex_pos & (tex->height - 1);
		tex_pos += step;
		put_pixel(game, x, y, get_texture_color(tex, tex_x, tex_y));
		y++;
	}
}

static void	draw_column(t_game *game, t_ray *ray, int x)
{
	int			y;
	t_texture	*tex;

	y = 0;
	while (y < ray->draw_start)
	{
		put_pixel(game, x, y, game->map->ceiling_color);
		y++;
	}
	tex = get_wall_texture(game, ray);
	draw_wall_texture(game, ray, x, tex);
	y = ray->draw_end + 1;
	while (y < WIN_HEIGHT)
	{
		put_pixel(game, x, y, game->map->floor_color);
		y++;
	}
}

void	render_3d(t_game *game)
{
	int		x;
	t_ray	ray;

	game->map->floor_color = 0x444444;
	game->map->ceiling_color = 0x87CEEB;
	x = 0;
	while (x < WIN_WIDTH)
	{
		init_ray(game, &ray, x);
		calc_step_and_side_dist(game, &ray);
		perform_dda(game, &ray);
		calc_wall_height(game, &ray);
		draw_column(game, &ray, x);
		x++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}
