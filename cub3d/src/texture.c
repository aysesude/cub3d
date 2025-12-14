/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 17:40:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/14 17:42:13 by aycami           ###   ########.fr       */
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

static int	load_single_texture(t_game *game, t_texture *tex, char *path)
{
	tex->img = mlx_xpm_file_to_image(game->mlx, path,
			&tex->width, &tex->height);
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
	return (1);
}
