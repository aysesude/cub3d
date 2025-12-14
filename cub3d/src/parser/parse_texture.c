/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 11:25:05 by aycami            #+#    #+#             */
/*   Updated: 2025/12/07 11:53:40 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	check_texture_path(char *path)
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

int	parse_north_texture(t_game *game, char *line)
{
	char	*path;

	if (game->map->no_texture)
		return (ft_putstr_fd("Error\nDuplicate NO texture\n", 2), -1);
	path = ft_strtrim(line + 2, " \t");
	if (!path || *path == '\0')
	{
		free(path);
		return (ft_putstr_fd("Error\nEmpty NO texture path\n", 2), -1);
	}
	if (!check_texture_path(path))
	{
		free(path);
		return (ft_putstr_fd("Error\nInvalid NO texture file\n", 2), -1);
	}
	game->map->no_texture = path;
	return (1);
}

int	parse_south_texture(t_game *game, char *line)
{
	char	*path;

	if (game->map->so_texture)
		return (ft_putstr_fd("Error\nDuplicate SO texture\n", 2), -1);
	path = ft_strtrim(line + 2, " \t");
	if (!path || *path == '\0')
	{
		free(path);
		return (ft_putstr_fd("Error\nEmpty SO texture path\n", 2), -1);
	}
	if (!check_texture_path(path))
	{
		free(path);
		return (ft_putstr_fd("Error\nInvalid SO texture file\n", 2), -1);
	}
	game->map->so_texture = path;
	return (1);
}

int	parse_west_texture(t_game *game, char *line)
{
	char	*path;

	if (game->map->we_texture)
		return (ft_putstr_fd("Error\nDuplicate WE texture\n", 2), -1);
	path = ft_strtrim(line + 2, " \t");
	if (!path || *path == '\0')
	{
		free(path);
		return (ft_putstr_fd("Error\nEmpty WE texture path\n", 2), -1);
	}
	if (!check_texture_path(path))
	{
		free(path);
		return (ft_putstr_fd("Error\nInvalid WE texture file\n", 2), -1);
	}
	game->map->we_texture = path;
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
