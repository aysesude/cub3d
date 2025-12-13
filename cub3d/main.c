#include "./include/cub3d.h"

int main(int argc, char **argv)
{
    t_game *game;
    
    if (argc != 2)
    {
        ft_putstr_fd("Error\nUsage: ./cub3D <map.cub>\n", 2);
        return (1);
    }
    
    game = init_game();
    if (!game)
    {
        ft_putstr_fd("Error\nMemory allocation failed\n", 2);
        return (1);
    }
    
    if (parse_map(game, argv[1]) == -1)
    {
        cleanup_game(game);
        return (1);
    }
    
    /* Map validation */
    if (validate_map(game) == -1)
    {
        cleanup_game(game);
        return (1);
    }
    
    if (init_graphics(game) == -1)
    {
        cleanup_game(game);
        return (1);
    }
    
    game_loop(game);
    cleanup_game(game);
    return (0);
}