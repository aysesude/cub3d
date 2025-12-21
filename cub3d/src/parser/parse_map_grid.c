/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_grid.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:10:00 by raktas            #+#    #+#             */
/*   Updated: 2025/12/21 16:02:14 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	free_grid(char **grid, int rows)
{
	int	i;

	if (!grid)
		return ;
	i = 0;
	while (i < rows)
	{
		free(grid[i]);
		i++;
	}
	free(grid);
}

static char	**copy_grid_rows(char **grid, char **new_grid, int rows)
{
	int	i;

	i = 0;
	while (i < rows)
	{
		new_grid[i] = grid[i];
		i++;
	}
	return (new_grid);
}

char	**add_map_line(char **grid, int rows, char *line)
{
	char	**new_grid;

	new_grid = malloc(sizeof(char *) * (rows + 1));
	if (!new_grid)
	{
		free_grid(grid, rows);
		return (NULL);
	}
	copy_grid_rows(grid, new_grid, rows);
	new_grid[rows] = ft_strdup(line);
	if (!new_grid[rows])
	{
		free(new_grid);
		free_grid(grid, rows);
		return (NULL);
	}
	free(grid);
	return (new_grid);
}

char	**init_map_grid(char *first_line)
{
	char	**grid;

	grid = malloc(sizeof(char *));
	if (!grid)
		return (NULL);
	grid[0] = ft_strdup(first_line);
	if (!grid[0])
		return (free(grid), NULL);
	return (grid);
}

int	process_map_line(t_game *game, char ***grid, int *rows, char *line)
{
	if (!is_map_line(line))
	{
		free_grid(*grid, *rows);
		return (free(line), ft_putstr_fd("Error\nInvalid map\n", 2), -1);
	}
	*grid = add_map_line(*grid, *rows, line);
	if (!*grid)
		return (free(line), ft_putstr_fd("Error\nMemory allocation failed\n",
				2), -1);
	if ((int)ft_strlen(line) > game->map->width)
		game->map->width = ft_strlen(line);
	(*rows)++;
	return (0);
}
