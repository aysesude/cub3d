#include "../include/cub3d.h"

static t_map *init_map(void)
{
    t_map *map;
    
    map = malloc(sizeof(t_map));
    if (!map)
        return (NULL);
    
    map->grid = NULL;
    map->width = 0;
    map->height = 0;
    map->no_texture = NULL;
    map->so_texture = NULL;
    map->we_texture = NULL;
    map->ea_texture = NULL;
    map->floor_color = -1;
    map->ceiling_color = -1;
    
    return (map);
}

static t_player *init_player(void)
{
    t_player *player;
    
    player = malloc(sizeof(t_player));
    if (!player)
        return (NULL);
    
    player->x = 0;
    player->y = 0;
    player->dir_x = -1;
    player->dir_y = 0;
    player->plane_x = 0;
    player->plane_y = 0.65;
    
    return (player);
}

t_game *init_game(void)
{
    t_game *game;
    
    game = malloc(sizeof(t_game));
    if (!game)
        return (NULL);
    game->mlx = NULL;
    game->win = NULL;
    game->img = NULL;
    game->addr = NULL;
    game->map = init_map();
    if (!game->map)
    {
        free(game);
        return (NULL);
    }
    game->player = init_player();
    if (!game->player)
    {
        free(game->map);
        free(game);
        return (NULL);
    }    
    return (game);
}