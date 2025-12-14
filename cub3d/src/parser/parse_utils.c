/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 11:25:08 by aycami            #+#    #+#             */
/*   Updated: 2025/12/07 11:53:45 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

char	*read_line(int fd)
{
	char	*line;
	int		len;

	line = get_next_line(fd);
	if (!line)
		return (NULL);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

int	is_map_line(char *line)
{
	int	i;
	int	has_content;

	i = 0;
	has_content = 0;
	while (line[i])
	{
		if (line[i] == '1' || line[i] == '0' || is_player_char(line[i]))
			has_content = 1;
		else if (line[i] != ' ' && line[i] != '\t')
			return (0);
		i++;
	}
	return (has_content);
}

int	check_file_extension(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 4)
		return (0);
	if (ft_strncmp(filename + len - 4, ".cub", 4) != 0)
		return (0);
	return (1);
}

int	validate_config(t_game *game)
{
	if (!game->map->no_texture)
		return (ft_putstr_fd("Error\nMissing NO texture\n", 2), -1);
	if (!game->map->so_texture)
		return (ft_putstr_fd("Error\nMissing SO texture\n", 2), -1);
	if (!game->map->we_texture)
		return (ft_putstr_fd("Error\nMissing WE texture\n", 2), -1);
	if (!game->map->ea_texture)
		return (ft_putstr_fd("Error\nMissing EA texture\n", 2), -1);
	if (game->map->floor_color == -1)
		return (ft_putstr_fd("Error\nMissing floor color\n", 2), -1);
	if (game->map->ceiling_color == -1)
		return (ft_putstr_fd("Error\nMissing ceiling color\n", 2), -1);
	return (0);
}

char	*skip_empty_lines(int fd)
{
	char	*line;
	char	*trimmed;

	while (1)
	{
		line = read_line(fd);
		if (!line)
			return (NULL);
		trimmed = ft_strtrim(line, " \t");
		if (trimmed && *trimmed != '\0')
		{
			free(trimmed);
			return (line);
		}
		free(trimmed);
		free(line);
	}
}
