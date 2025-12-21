/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_walls.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 11:25:37 by aycami            #+#    #+#             */
/*   Updated: 2025/12/21 19:43:40 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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

void	write_number(int n)
{
	char	buf[12];
	int		i;

	i = 0;
	if (n == 0)
		buf[i++] = '0';
	while (n > 0)
	{
		buf[i++] = '0' + (n % 10);
		n /= 10;
	}
	buf[i] = '\0';
	while (--i >= 0)
		write(2, &buf[i], 1);
}
