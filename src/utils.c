#include "../include/cub3d.h"

void cleanup_game(t_game *game)
{
	if (!game)
		return;
	
	if (game->map)
	{
		if (game->map->grid)
		{
			int i = 0;
			while (i < game->map->height)
			{
				free(game->map->grid[i]);
				i++;
			}
			free(game->map->grid);
		}
		free(game->map->no_texture);
		free(game->map->so_texture);
		free(game->map->we_texture);
		free(game->map->ea_texture);
		free(game->map);
	}
	if (game->player)
		free(game->player);
	
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
		mlx_destroy_display(game->mlx);
	free(game);
}
