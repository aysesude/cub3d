/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 11:25:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/14 18:09:02 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	parse_identifier(t_game *game, char *line)
{
	char	*trimmed;
	int		result;

	trimmed = ft_strtrim(line, " \t");
	if (!trimmed || *trimmed == '\0')
		return (free(trimmed), 0);
	if (ft_strlen(trimmed) >= 3 && trimmed[0] == 'N' && trimmed[1] == 'O'
		&& (trimmed[2] == ' ' || trimmed[2] == '\t'))
		result = parse_north_texture(game, trimmed);
	else if (ft_strlen(trimmed) >= 3 && trimmed[0] == 'S' && trimmed[1] == 'O'
		&& (trimmed[2] == ' ' || trimmed[2] == '\t'))
		result = parse_south_texture(game, trimmed);
	else if (ft_strlen(trimmed) >= 3 && trimmed[0] == 'W' && trimmed[1] == 'E'
		&& (trimmed[2] == ' ' || trimmed[2] == '\t'))
		result = parse_west_texture(game, trimmed);
	else if (ft_strlen(trimmed) >= 3 && trimmed[0] == 'E' && trimmed[1] == 'A'
		&& (trimmed[2] == ' ' || trimmed[2] == '\t'))
		result = parse_east_texture(game, trimmed);
	else if (ft_strlen(trimmed) >= 2 && trimmed[0] == 'F'
		&& (trimmed[1] == ' ' || trimmed[1] == '\t'))
		result = parse_floor_color(game, trimmed);
	else if (ft_strlen(trimmed) >= 2 && trimmed[0] == 'C'
		&& (trimmed[1] == ' ' || trimmed[1] == '\t'))
		result = parse_ceiling_color(game, trimmed);
	else if (is_map_line(trimmed))
		result = 2;
	else
		result = (ft_putstr_fd("Error\nInvalid identifier\n", 2), -1);
	return (free(trimmed), result);
}

static int	parse_config_section(t_game *game, int fd, char **first_map_line)
{
	char	*line;
	int		result;

	while (1)
	{
		line = read_line(fd);
		if (!line)
			return (ft_putstr_fd("Error\nNo map found\n", 2), -1);
		result = parse_identifier(game, line);
		if (result == -1)
			return (free(line), -1);
		if (result == 2)
		{
			*first_map_line = line;
			return (0);
		}
		free(line);
	}
}

static void	free_grid(char **grid, int rows)
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

static char	**add_map_line(char **grid, int rows, char *line)
{
	char	**new_grid;
	int		i;

	new_grid = malloc(sizeof(char *) * (rows + 1));
	if (!new_grid)
	{
		free_grid(grid, rows); 
		return (NULL);
	}
	i = 0;
	while (i < rows)
	{
		new_grid[i] = grid[i];
		i++;
	}
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

static int	parse_map_section(t_game *game, int fd, char *first_line)
{
	char	**grid;
	char	*line;
	int		rows;

	grid = malloc(sizeof(char *));
	if (!grid)
		return (-1);
	grid[0] = ft_strdup(first_line);
	if (!grid[0])  
		return (free(grid), -1);
	rows = 1;
	game->map->width = ft_strlen(first_line);
	while (1)
	{
		line = read_line(fd);
		if (!line)
			break ;
		if (!is_map_line(line))
		{
			free_grid(grid, rows); 
			return (free(line), ft_putstr_fd("Error\nInvalid map\n", 2), -1);
		}
		grid = add_map_line(grid, rows, line);
		if (!grid)  
			return (free(line), ft_putstr_fd("Error\nMemory allocation failed\n", 2), -1);
		if ((int)ft_strlen(line) > game->map->width)
			game->map->width = ft_strlen(line);
		rows++;
		free(line);
	}
	game->map->grid = grid;
	game->map->height = rows;
	return (0);
}

int	parse_map(t_game *game, char *filename)
{
	int		fd;
	char	*first_map_line;

	if (!check_file_extension(filename))
		return (ft_putstr_fd("Error\nFile must be .cub\n", 2), -1);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (ft_putstr_fd("Error\nCannot open file\n", 2), -1);
	if (parse_config_section(game, fd, &first_map_line) == -1)
		return (close(fd), -1);
	if (validate_config(game) == -1)
		return (free(first_map_line), close(fd), -1);
	if (parse_map_section(game, fd, first_map_line) == -1)
		return (free(first_map_line), close(fd), -1);
	free(first_map_line);
	close(fd);
	return (find_player(game));
}