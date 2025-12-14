/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_walls.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 11:25:37 by aycami            #+#    #+#             */
/*   Updated: 2025/12/07 11:26:20 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	check_first_row(t_game *game)
{
	int	x;

	x = 0;
	while (x < (int)ft_strlen(game->map->grid[0]))
	{
		if (game->map->grid[0][x] != '1' && game->map->grid[0][x] != ' ')
		{
			ft_putstr_fd("Error\nFirst row must be walls/spaces\n", 2);
			return (-1);
		}
		x++;
	}
	return (0);
}

static int	check_last_row(t_game *game)
{
	int	x;
	int	last;

	last = game->map->height - 1;
	x = 0;
	while (x < (int)ft_strlen(game->map->grid[last]))
	{
		if (game->map->grid[last][x] != '1'
			&& game->map->grid[last][x] != ' ')
		{
			ft_putstr_fd("Error\nLast row must be walls/spaces\n", 2);
			return (-1);
		}
		x++;
	}
	return (0);
}

static int	find_first_char(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] == ' ')
		i++;
	return (i);
}

static int	find_last_char(char *line)
{
	int	i;

	i = ft_strlen(line) - 1;
	while (i >= 0 && line[i] == ' ')
		i--;
	return (i);
}

static int	check_row_edges(t_game *game, int y)
{
	int		first;
	int		last;
	char	*line;

	line = game->map->grid[y];
	if (ft_strlen(line) == 0)
		return (0);
	first = find_first_char(line);
	last = find_last_char(line);
	if (first < (int)ft_strlen(line) && line[first] != '1')
	{
		ft_putstr_fd("Error\nRow must start with wall\n", 2);
		return (-1);
	}
	if (last >= 0 && line[last] != '1')
	{
		ft_putstr_fd("Error\nRow must end with wall\n", 2);
		return (-1);
	}
	return (0);
}

int	validate_map_walls(t_game *game)
{
	int	y;

	if (check_first_row(game) == -1)
		return (-1);
	if (check_last_row(game) == -1)
		return (-1);
	y = 0;
	while (y < game->map->height)
	{
		if (check_row_edges(game, y) == -1)
			return (-1);
		y++;
	}
	return (0);
}
