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

#include "../include/cub3d_bonus.h"

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

static t_texture	*check_open_door_wall(t_game *game, t_ray *ray)
{
	int		mx;
	int		my;
	size_t	len_m;
	int		has_open_n;
	int		has_open_e;

	mx = ray->map_x;
	my = ray->map_y;
	len_m = ft_strlen(game->map->grid[my]);
	has_open_n = 0;
	has_open_e = 0;
	if (my - 1 >= 0)
	{
		if ((size_t)mx < ft_strlen(game->map->grid[my - 1])
			&& game->map->grid[my - 1][mx] == 'o')
			has_open_n = 1;
	}
	if ((size_t)(mx + 1) < len_m && game->map->grid[my][mx + 1] == 'o')
		has_open_e = 1;
	if (ray->side == 1 && ray->ray_dir_y > 0 && has_open_n)
		return (&game->tex_do);
	if (ray->side == 0 && ray->ray_dir_x < 0 && has_open_e)
		return (&game->tex_do);
	return (get_wall_texture(game, ray));
}

static t_texture	*get_door_texture(t_game *game, t_ray *ray)
{
	if (!game->map->do_texture)
		return (get_wall_texture(game, ray));
	if (game->map->grid[ray->map_y][ray->map_x] == 'D')
		return (&game->tex_do);
	if (game->map->grid[ray->map_y][ray->map_x] == '1')
		return (check_open_door_wall(game, ray));
	return (get_wall_texture(game, ray));
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
	tex = get_door_texture(game, ray);
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
