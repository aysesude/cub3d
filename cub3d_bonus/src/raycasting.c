/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 17:40:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/14 17:42:14 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

/*
** init_ray - Ekrandaki bir sütun için ışın başlatır
** @game: Oyun yapısı
** @ray: Işın bilgisi (çıktı)
** @x: Ekran sütunu (piksel)
**
** Oyuncunun bakış açısına göre ışının yönünü ve harita üzerindeki başlangıç noktasını hesaplar.
*/
void	init_ray(t_game *game, t_ray *ray, int x)
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

/*
** calc_step_and_side_dist - DDA için adım ve ilk yan mesafeleri hesaplar
** @game: Oyun yapısı
** @ray: Işın bilgisi
**
** Işının hangi yöne gideceğini ve ilk grid kenarına olan mesafeyi ayarlar.
*/
void	calc_step_and_side_dist(t_game *game, t_ray *ray)
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

/*
** perform_dda - DDA algoritması ile ışını haritada ilerletir
** @game: Oyun yapısı
** @ray: Işın bilgisi
**
** Işın, duvar veya kapıya çarpana kadar grid hücrelerinde adım atar.
*/
void	perform_dda(t_game *game, t_ray *ray)
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
		if (game->map->grid[ray->map_y][ray->map_x] == '1'
			|| game->map->grid[ray->map_y][ray->map_x] == 'D')
			ray->hit = 1;
	}
}

/*
** calc_wall_height - Duvara olan mesafeye göre ekranda çizilecek yükseklikleri hesaplar
** @game: Oyun yapısı
** @ray: Işın bilgisi
**
** Perspektif düzeltmesiyle duvarın ekrandaki yüksekliğini ve çizim aralığını belirler.
*/
void	calc_wall_height(t_game *game, t_ray *ray)
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

/*
** get_wall_texture - Işının çarptığı duvarın yönüne göre doğru texture'ı döner
** @game: Oyun yapısı
** @ray: Işın bilgisi
**
** Yan (side) ve ışın yönüne göre kuzey, güney, doğu, batı duvarı texture'ı seçilir.
*/
t_texture	*get_wall_texture(t_game *game, t_ray *ray)
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
