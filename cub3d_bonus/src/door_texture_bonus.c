/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

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

t_texture	*get_door_texture(t_game *game, t_ray *ray)
{
	if (!game->map->do_texture)
		return (get_wall_texture(game, ray));
	if (game->map->grid[ray->map_y][ray->map_x] == 'D')
		return (&game->tex_do);
	if (game->map->grid[ray->map_y][ray->map_x] == '1')
		return (check_open_door_wall(game, ray));
	return (get_wall_texture(game, ray));
}
