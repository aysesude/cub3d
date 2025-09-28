#include "../include/cub3d.h"

int parse_map(t_game *game, char *filename)
{
    if (!filename || !game)
    {
        ft_putstr_fd("Error\nInvalid filename\n", 2);
        return (-1);
    }
    int len = 0;
    while (filename[len])
        len++;
    
    if (len < 4)
    {
        ft_putstr_fd("Error\nInvalid file extension\n", 2);
        return (-1);
    }
    
    if (filename[len-4] != '.' || filename[len-3] != 'c' || 
        filename[len-2] != 'u' || filename[len-1] != 'b')
    {
        ft_putstr_fd("Error\nFile must have .cub extension\n", 2);
        return (-1);
    }
    
    // Parse loadingg...

    game->map->width = 0;
    game->map->height = 0;
    return (0);
}