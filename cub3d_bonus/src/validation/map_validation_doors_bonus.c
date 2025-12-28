/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_doors.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 00:00:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/13 00:00:00 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d_bonus.h"

static int	is_border_or_outside(t_game *game, int x, int y)
{
	if (y <= 0 || y >= game->map->height - 1)
		return (1);
	if (x <= 0)
		return (1);
	if (x >= (int)ft_strlen(game->map->grid[y]) - 1)
		return (1);
	return (0);
}

static int	check_door_adjacent(t_game *game, int x, int y)
{
	char	dirs[4];

	dirs[0] = get_map_char(game, x, y - 1);
	dirs[1] = get_map_char(game, x, y + 1);
	dirs[2] = get_map_char(game, x - 1, y);
	dirs[3] = get_map_char(game, x + 1, y);
	if (dirs[0] == '\0' || dirs[1] == '\0' || dirs[2] == '\0' || dirs[3] == '\0'
		|| dirs[0] == ' ' || dirs[1] == ' ' || dirs[2] == ' ' || dirs[3] == ' ')
		return (ft_putstr_fd("Error\nDoor adjacent to outside or space\n", 2)
			, -1);
	if (dirs[0] == '1' && dirs[1] == '1')
	{
		if (!is_walkable(dirs[2]) || !is_walkable(dirs[3]))
			return (ft_putstr_fd("Error\nDoor NS must have walkable e/w\n", 2)
				, -1);
	}
	else if (dirs[2] == '1' && dirs[3] == '1')
	{
		if (!is_walkable(dirs[0]) || !is_walkable(dirs[1]))
			return (ft_putstr_fd("Error\nDoor EW must have walkable n/s\n", 2)
				, -1);
	}
	else
		return (ft_putstr_fd("Error\nInvalid door orientation\n", 2), -1);
	return (0);
}

static int	validate_single_door(t_game *game, int x, int y)
{
	if (is_border_or_outside(game, x, y))
		return (ft_putstr_fd("Error\nDoor on map border\n", 2), -1);
	return (check_door_adjacent(game, x, y));
}

int	validate_map_doors(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < game->map->height)
	{
		x = 0;
		while (x < (int)ft_strlen(game->map->grid[y]))
		{
			if (game->map->grid[y][x] == 'D')
			{
				if (validate_single_door(game, x, y) == -1)
					return (-1);
			}
			x++;
		}
		y++;
	}
	return (0);
}
