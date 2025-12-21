/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_section.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:11:00 by raktas            #+#    #+#             */
/*   Updated: 2025/12/21 16:02:17 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	parse_map_section(t_game *game, int fd, char *first_line)
{
	char	**grid;
	char	*line;
	int		rows;

	grid = init_map_grid(first_line);
	if (!grid)
		return (-1);
	rows = 1;
	game->map->width = ft_strlen(first_line);
	while (1)
	{
		line = read_line(fd);
		if (!line)
			break ;
		if (process_map_line(game, &grid, &rows, line) == -1)
			return (-1);
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
