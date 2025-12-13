/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_3d_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 03:11:41 by aycami            #+#    #+#             */
/*   Updated: 2025/12/07 11:48:57 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	get_texture_color(t_texture *tex, int x, int y)
{
	char	*pixel;

	if (x < 0 || x >= tex->width || y < 0 || y >= tex->height)
		return (0);
	pixel = tex->addr + (y * tex->line_length + x * (tex->bits_per_pixel / 8));
	return (*(unsigned int *)pixel);
}

int	load_single_texture(t_game *game, t_texture *tex, char *path)
{
	tex->img = mlx_xpm_file_to_image(game->mlx, path, &tex->width, &tex->height);
	if (!tex->img)
	{
		printf("Error: Failed to load texture: %s\n", path);
		return (0);
	}
	tex->addr = mlx_get_data_addr(tex->img, &tex->bits_per_pixel,
			&tex->line_length, &tex->endian);
	return (1);
}

int	load_textures(t_game *game)
{
	if (!load_single_texture(game, &game->tex_no, game->map->no_texture))
		return (0);
	if (!load_single_texture(game, &game->tex_so, game->map->so_texture))
		return (0);
	if (!load_single_texture(game, &game->tex_we, game->map->we_texture))
		return (0);
	if (!load_single_texture(game, &game->tex_ea, game->map->ea_texture))
		return (0);
	if (game->map->do_texture)
	{
		if (!load_single_texture(game, &game->tex_do, game->map->do_texture))
			return (0);
	}
	return (1);
}

static void	init_ray(t_game *game, t_ray *ray, int x)
{
	double	camera_x;

	camera_x = 2 * x / (double)WIN_WIDTH - 1;
	ray->ray_dir_x = game->player->dir_x + game->player->plane_x * camera_x;
	ray->ray_dir_y = game->player->dir_y + game->player->plane_y * camera_x;
	ray->map_x = (int)game->player->x;
	ray->map_y = (int)game->player->y;
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(1.0 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(1.0 / ray->ray_dir_y);
	ray->hit = 0;
	ray->side = 0;
}

static void	calc_step_and_side_dist(t_game *game, t_ray *ray)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (game->player->x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - game->player->x)
			* ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (game->player->y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - game->player->y)
			* ray->delta_dist_y;
	}
}

static void	perform_dda(t_game *game, t_ray *ray)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (game->map->grid[ray->map_y][ray->map_x] == '1' ||
			game->map->grid[ray->map_y][ray->map_x] == 'D')
			ray->hit = 1;
	}
}

static void	calc_wall_height(t_game *game, t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - game->player->x
				+ (1 - ray->step_x) / 2.0) / ray->ray_dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - game->player->y
				+ (1 - ray->step_y) / 2.0) / ray->ray_dir_y;
	ray->line_height = (int)(WIN_HEIGHT / ray->perp_wall_dist);
	ray->draw_start = -ray->line_height / 2 + WIN_HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + WIN_HEIGHT / 2;
	if (ray->draw_end >= WIN_HEIGHT)
		ray->draw_end = WIN_HEIGHT - 1;
}

static t_texture	*get_wall_texture(t_game *game, t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->ray_dir_x > 0)
			return (&game->tex_ea);
		else
			return (&game->tex_we);
	}
	else
	{
		if (ray->ray_dir_y > 0)
			return (&game->tex_so);
		else
			return (&game->tex_no);
	}
}

static void	draw_column(t_game *game, t_ray *ray, int x)
{
	int			y;
	t_texture	*tex;
	int			tex_x;
	int			tex_y;
	double		wall_x;
	double		step;
	double		tex_pos;

	y = 0;
	while (y < ray->draw_start)
	{
		put_pixel(game, x, y, game->map->ceiling_color);
		y++;
	}
	if (game->map->grid[ray->map_y][ray->map_x] == 'D' && game->map->do_texture)
		tex = &game->tex_do;
	else
		tex = get_wall_texture(game, ray);
	if (ray->side == 0)
		wall_x = game->player->y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		wall_x = game->player->x + ray->perp_wall_dist * ray->ray_dir_x;
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * (double)tex->width);
	if ((ray->side == 0 && ray->ray_dir_x > 0)
		|| (ray->side == 1 && ray->ray_dir_y < 0))
		tex_x = tex->width - tex_x - 1;
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
