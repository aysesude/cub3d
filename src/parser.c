#include "../include/cub3d.h"

static char *read_line(int fd)
{
    char *line = get_next_line(fd);
    if (!line)
        return (NULL);
    // satır sonundaki \n'yi temizle
    int len = ft_strlen(line);
    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';
    return (line);
}

int parse_map(t_game *game, char *filename)
{
    int     fd;
    char    *line;
    char    **grid = NULL;
    int     rows = 0;
    int     cols = 0;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (ft_putstr_fd("Error\nFile open failed\n", 2), -1);

    if (!filename || !game)
    {
        ft_putstr_fd("Error\nInvalid filename\n", 2);
        return (-1);
    }
    int len = 0;
    while (filename[len])
        len++;
    
    if (len < 4) // Map .cub formatında olmalı.
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

    while ((line = read_line(fd)))
    {
        grid = realloc(grid, sizeof(char *) * (rows + 1));
        grid[rows] = ft_strdup(line);
        if ((int)ft_strlen(line) > cols)
            cols = ft_strlen(line);
        rows++;
        free(line);
    }
    close(fd);

    game->map->grid = grid;
    game->map->height = rows;
    game->map->width = cols;

    // Oyuncu pozisyonunu bul
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < (int)ft_strlen(grid[y]); x++)
        {
            if (grid[y][x] == 'N')
            {
                game->player->x = x + 0.5;
                game->player->y = y + 0.5;
                return (0);
            }
        }
    }
    return (0);
}