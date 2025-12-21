/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_identifiers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 11:25:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/21 16:02:11 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	check_texture_identifier(t_game *game, char *trimmed)
{
	if (ft_strlen(trimmed) >= 3 && trimmed[0] == 'N' && trimmed[1] == 'O'
		&& (trimmed[2] == ' ' || trimmed[2] == '\t'))
		return (parse_north_texture(game, trimmed));
	else if (ft_strlen(trimmed) >= 3 && trimmed[0] == 'S' && trimmed[1] == 'O'
		&& (trimmed[2] == ' ' || trimmed[2] == '\t'))
		return (parse_south_texture(game, trimmed));
	else if (ft_strlen(trimmed) >= 3 && trimmed[0] == 'W' && trimmed[1] == 'E'
		&& (trimmed[2] == ' ' || trimmed[2] == '\t'))
		return (parse_west_texture(game, trimmed));
	else if (ft_strlen(trimmed) >= 3 && trimmed[0] == 'E' && trimmed[1] == 'A'
		&& (trimmed[2] == ' ' || trimmed[2] == '\t'))
		return (parse_east_texture(game, trimmed));
	return (-2);
}

static int	check_color_identifier(t_game *game, char *trimmed)
{
	if (ft_strlen(trimmed) >= 2 && trimmed[0] == 'F'
		&& (trimmed[1] == ' ' || trimmed[1] == '\t'))
		return (parse_floor_color(game, trimmed));
	else if (ft_strlen(trimmed) >= 2 && trimmed[0] == 'C'
		&& (trimmed[1] == ' ' || trimmed[1] == '\t'))
		return (parse_ceiling_color(game, trimmed));
	return (-2);
}

int	parse_identifier(t_game *game, char *line)
{
	char	*trimmed;
	int		result;

	trimmed = ft_strtrim(line, " \t");
	if (!trimmed || *trimmed == '\0')
		return (free(trimmed), 0);
	result = check_texture_identifier(game, trimmed);
	if (result != -2)
		return (free(trimmed), result);
	result = check_color_identifier(game, trimmed);
	if (result != -2)
		return (free(trimmed), result);
	if (is_map_line(trimmed))
		result = 2;
	else
		result = (ft_putstr_fd("Error\nInvalid identifier\n", 2), -1);
	return (free(trimmed), result);
}

int	parse_config_section(t_game *game, int fd, char **first_map_line)
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
