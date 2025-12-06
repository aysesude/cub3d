#include "../include/cub3d.h"

void	draw_vertical_line(t_game *game, int x, int start, int end, int color)
{
	int	y;

	if (start < 0)
		start = 0;
	if (end >= WIN_HEIGHT)
		end = WIN_HEIGHT - 1;
	y = start;
	while (y <= end)
	{
		put_pixel(game, x, y, color);
		y++;
	}
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
		if (game->map->grid[ray->map_y][ray->map_x] == '1')
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

static void	draw_column(t_game *game, t_ray *ray, int x)
{
	int	y;
	int	wall_color;

	y = 0;
	while (y < ray->draw_start)
	{
		put_pixel(game, x, y, game->map->ceiling_color);
		y++;
	}
	if (ray->side == 1)
		wall_color = 0x888888;
	else
		wall_color = 0xBBBBBB;
	draw_vertical_line(game, x, ray->draw_start, ray->draw_end, wall_color);
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
