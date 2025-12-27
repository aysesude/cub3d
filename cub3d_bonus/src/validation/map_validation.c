/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 11:25:40 by aycami            #+#    #+#             */
/*   Updated: 2025/12/07 11:26:15 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d_bonus.h"

int	is_valid_map_char(char c)
{
	return (c == '0' || c == '1' || c == 'D' || c == ' ' || is_player_char(c));
}

int	validate_map_chars(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < game->map->height)
	{
		x = 0;
		while (x < (int)ft_strlen(game->map->grid[y]))
		{
			if (!is_valid_map_char(game->map->grid[y][x]))
			{
				ft_putstr_fd("Error\nInvalid character in map\n", 2);
				return (-1);
			}
			x++;
		}
		y++;
	}
	return (0);
}

char	get_map_char(t_game *game, int x, int y)
{
	if (y < 0 || y >= game->map->height)
		return ('\0');
	if (x < 0 || x >= (int)ft_strlen(game->map->grid[y]))
		return ('\0');
	return (game->map->grid[y][x]);
}

int	is_walkable(char c)
{
	return (c == '0' || is_player_char(c));
}

int	validate_map(t_game *game)
{
	if (validate_map_chars(game) == -1)
		return (-1);
	if (validate_map_walls(game) == -1)
		return (-1);
	if (validate_map_doors(game) == -1)
		return (-1);
	if (validate_map_closed(game) == -1)
		return (-1);
	return (0);
}
