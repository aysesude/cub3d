/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:12:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/21 16:02:23 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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
