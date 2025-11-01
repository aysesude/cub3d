#include "../include/cub3d.h"

static char *read_line(int fd)
{
    char *line = get_next_line(fd);
    if (!line)
        return (NULL);
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
        close(fd);
        return (-1);
    }
    int len = 0;
    while (filename[len])
        len++;
    
    if (len < 4)
    {
        ft_putstr_fd("Error\nInvalid file extension\n", 2);
        close(fd);
        return (-1);
    }
    
    if (filename[len-4] != '.' || filename[len-3] != 'c' || 
        filename[len-2] != 'u' || filename[len-1] != 'b')
    {
        ft_putstr_fd("Error\nFile must have .cub extension\n", 2);
        close(fd);
        return (-1);
    }

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

    /* Oyuncu pozisyonunu ve yönünü bul */
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < (int)ft_strlen(grid[y]); x++)
        {
            char c = grid[y][x];
            if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
            {
                /* Oyun içi pozisyon: hücrenin ortası */
                game->player->x = x + 0.5;
                game->player->y = y + 0.5;
                /* Yön vektörünü ata */
                if (c == 'N') { game->player->dir_x = 0;  game->player->dir_y = -1; }
                if (c == 'S') { game->player->dir_x = 0;  game->player->dir_y = 1;  }
                if (c == 'E') { game->player->dir_x = 1;  game->player->dir_y = 0;  }
                if (c == 'W') { game->player->dir_x = -1; game->player->dir_y = 0;  }

                /* Kamera düzlemi (FOV yaklaşık 66°) - 0.66 civarı iyi çalışır */
                /* plane is perpendicular to dir: ( -dir_y * scale, dir_x * scale ) */
                double scale = 0.66;
                game->player->plane_x = -game->player->dir_y * scale;
                game->player->plane_y = game->player->dir_x * scale;

                /* Harita üzerindeki oyuncu harfini 0'la değiştir (yürünebilir) */
                game->map->grid[y][x] = '0';
                goto found_player;
            }
        }
    }

found_player:
    return (0);
}
