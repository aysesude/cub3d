#include "../../include/cub3d.h"

static void	set_player_direction(t_player *player, char direction)
{
	if (direction == 'N')
	{
		player->dir_x = 0;
		player->dir_y = -1;
	}
	else if (direction == 'S')
	{
		player->dir_x = 0;
		player->dir_y = 1;
	}
	else if (direction == 'E')
	{
		player->dir_x = 1;
		player->dir_y = 0;
	}
	else if (direction == 'W')
	{
		player->dir_x = -1;
		player->dir_y = 0;
	}
}

static void	set_player_plane(t_player *player)
{
	double	scale;

	scale = 0.66;
	player->plane_x = -player->dir_y * scale;
	player->plane_y = player->dir_x * scale;
}

static void	init_player_pos(t_game *game, int x, int y, char dir)
{
	game->player->x = x + 0.5;
	game->player->y = y + 0.5;
	set_player_direction(game->player, dir);
	set_player_plane(game->player);
	game->map->grid[y][x] = '0';
}

int	is_player_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

int	find_player(t_game *game)
{
	int		y;
	int		x;
	int		found;

	y = 0;
	found = 0;
	while (y < game->map->height)
	{
		x = 0;
		while (x < (int)ft_strlen(game->map->grid[y]))
		{
			if (is_player_char(game->map->grid[y][x]))
			{
				if (found)
					return (ft_putstr_fd("Error\nMultiple players\n", 2), -1);
				init_player_pos(game, x, y, game->map->grid[y][x]);
				found = 1;
			}
			x++;
		}
		y++;
	}
	if (!found)
		return (ft_putstr_fd("Error\nNo player found\n", 2), -1);
	return (0);
}