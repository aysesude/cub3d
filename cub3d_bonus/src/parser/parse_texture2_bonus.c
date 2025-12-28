/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d_bonus.h"

int	check_texture_path(char *path)
{
	int	fd;
	int	len;

	if (!path)
		return (0);
	len = ft_strlen(path);
	if (len < 4)
		return (0);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}

int	parse_east_texture(t_game *game, char *line)
{
	char	*path;

	if (game->map->ea_texture)
		return (ft_putstr_fd("Error\nDuplicate EA texture\n", 2), -1);
	path = ft_strtrim(line + 2, " \t");
	if (!path || *path == '\0')
	{
		free(path);
		return (ft_putstr_fd("Error\nEmpty EA texture path\n", 2), -1);
	}
	if (!check_texture_path(path))
	{
		free(path);
		return (ft_putstr_fd("Error\nInvalid EA texture file\n", 2), -1);
	}
	game->map->ea_texture = path;
	return (1);
}

int	parse_door_texture(t_game *game, char *line)
{
	char	*path;

	if (game->map->do_texture)
		return (ft_putstr_fd("Error\nDuplicate DO texture\n", 2), -1);
	path = ft_strtrim(line + 2, " \t");
	if (!path || *path == '\0')
	{
		free(path);
		return (ft_putstr_fd("Error\nEmpty DO texture path\n", 2), -1);
	}
	if (!check_texture_path(path))
	{
		free(path);
		return (ft_putstr_fd("Error\nInvalid DO texture file\n", 2), -1);
	}
	game->map->do_texture = path;
	return (1);
}
