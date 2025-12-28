/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_closed.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 11:25:35 by aycami            #+#    #+#             */
/*   Updated: 2025/12/28 14:18:19 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	check_surrounded(t_game *game, int x, int y)
{
	char	c;
	int		directions[4][2];
	int		i;

	directions[0][0] = -1;
	directions[0][1] = 0;
	directions[1][0] = 1;
	directions[1][1] = 0;
	directions[2][0] = 0;
	directions[2][1] = -1;
	directions[3][0] = 0;
	directions[3][1] = 1;
	i = 0;
	while (i < 4)
	{
		c = get_map_char(game, x + directions[i][0], y + directions[i][1]);
		if (c == '\0' || c == ' ')
			return (-1);
		i++;
	}
	return (0);
}

static void	print_position_error(int x, int y)
{
	ft_putstr_fd("Error\nMap not closed at (", 2);
	write_number(x);
	ft_putstr_fd(", ", 2);
	write_number(y);
	ft_putstr_fd(")\n", 2);
}

int	validate_map_closed(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < game->map->height)
	{
		x = 0;
		while (x < (int)ft_strlen(game->map->grid[y]))
		{
			if (game->map->grid[y][x] == '0')
			{
				if (check_surrounded(game, x, y) == -1)
				{
					print_position_error(x, y);
					return (-1);
				}
			}
			x++;
		}
		y++;
	}
	return (0);
}
