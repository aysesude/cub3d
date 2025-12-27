/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_grid.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d_bonus.h"

static char	**add_map_line(char **grid, int rows, char *line)
{
	char	**new_grid;
	int		i;

	new_grid = malloc(sizeof(char *) * (rows + 1));
	if (!new_grid)
		return (NULL);
	i = 0;
	while (i < rows)
	{
		new_grid[i] = grid[i];
		i++;
	}
	new_grid[rows] = ft_strdup(line);
	free(grid);
	return (new_grid);
}

static int	read_map_lines(t_game *game, int fd, char ***grid, int *rows)
{
	char	*line;

	while (1)
	{
		line = read_line(fd);
		if (!line)
			break ;
		if (!is_map_line(line))
			return (free(line), ft_putstr_fd("Error\nInvalid map\n", 2), -1);
		*grid = add_map_line(*grid, *rows, line);
		if ((int)ft_strlen(line) > game->map->width)
			game->map->width = ft_strlen(line);
		(*rows)++;
		free(line);
	}
	return (0);
}

int	parse_map_section(t_game *game, int fd, char *first_line)
{
	char	**grid;
	int		rows;

	grid = malloc(sizeof(char *));
	if (!grid)
		return (-1);
	grid[0] = ft_strdup(first_line);
	rows = 1;
	game->map->width = ft_strlen(first_line);
	if (read_map_lines(game, fd, &grid, &rows) == -1)
		return (-1);
	game->map->grid = grid;
	game->map->height = rows;
	return (0);
}
