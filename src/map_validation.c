#include "../include/cub3d.h"

/* Karakterin geçerli map karakteri olup olmadığını kontrol et */
static int is_valid_char(char c)
{
    return (c == '0' || c == '1' || c == ' ' || 
            c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

/* Map'teki tüm karakterlerin geçerli olup olmadığını kontrol et */
int validate_map_chars(t_game *game)
{
    int y = 0;
    
    while (y < game->map->height)
    {
        int x = 0;
        while (x < (int)ft_strlen(game->map->grid[y]))
        {
            if (!is_valid_char(game->map->grid[y][x]))
            {
                ft_putstr_fd("Error\nInvalid character in map: '", 2);
                write(2, &game->map->grid[y][x], 1);
                ft_putstr_fd("'\n", 2);
                return (-1);
            }
            x++;
        }
        y++;
    }
    return (0);
}

/* Her satırın ilk ve son geçerli karakterinin duvar olduğunu kontrol et */
int validate_map_walls(t_game *game)
{
    int y = 0;
    
    /* İlk ve son satırda sadece duvar ve boşluk olmalı */
    int x = 0;
    while (x < (int)ft_strlen(game->map->grid[0]))
    {
        if (game->map->grid[0][x] != '1' && game->map->grid[0][x] != ' ')
        {
            ft_putstr_fd("Error\nFirst row must be walls or spaces\n", 2);
            return (-1);
        }
        x++;
    }
    
    x = 0;
    while (x < (int)ft_strlen(game->map->grid[game->map->height - 1]))
    {
        if (game->map->grid[game->map->height - 1][x] != '1' && 
            game->map->grid[game->map->height - 1][x] != ' ')
        {
            ft_putstr_fd("Error\nLast row must be walls or spaces\n", 2);
            return (-1);
        }
        x++;
    }
    
    /* Her satırın ilk ve son karakteri kontrol */
    while (y < game->map->height)
    {
        int len = ft_strlen(game->map->grid[y]);
        if (len > 0)
        {
            /* İlk karakter (boşluk değil) */
            int first = 0;
            while (first < len && game->map->grid[y][first] == ' ')
                first++;
            if (first < len && game->map->grid[y][first] != '1')
            {
                ft_putstr_fd("Error\nMap row must start with wall after spaces\n", 2);
                return (-1);
            }
            
            /* Son karakter (boşluk değil) */
            int last = len - 1;
            while (last >= 0 && game->map->grid[y][last] == ' ')
                last--;
            if (last >= 0 && game->map->grid[y][last] != '1')
            {
                ft_putstr_fd("Error\nMap row must end with wall before spaces\n", 2);
                return (-1);
            }
        }
        y++;
    }
    
    return (0);
}

/* Map'in belirli pozisyonundaki karakteri al (bounds check ile) */
static char get_map_char(t_game *game, int x, int y)
{
    if (y < 0 || y >= game->map->height)
        return ('\0');
    if (x < 0 || x >= (int)ft_strlen(game->map->grid[y]))
        return ('\0');
    return (game->map->grid[y][x]);
}

/* 0 veya oyuncu pozisyonunun etrafının kapalı olduğunu kontrol et */
static int check_surrounded(t_game *game, int x, int y)
{
    char c;
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    int i = 0;
    
    while (i < 4)
    {
        c = get_map_char(game, x + dx[i], y + dy[i]);
        
        /* Sınır dışı veya boşluk = hata */
        if (c == '\0' || c == ' ')
        {
            /* Ama boşluğun etrafında da boşluk varsa tamam */
            /* Sadece 0'ın yanında boşluk varsa hata */
            return (-1);
        }
        i++;
    }
    return (0);
}

/* Map'in duvarlarla çevrili olup olmadığını kontrol et */
int validate_map_closed(t_game *game)
{
    int y = 0;
    
    /* Her 0 ve oyuncu pozisyonunun 4 yönde duvar veya 0 olmalı */
    while (y < game->map->height)
    {
        int x = 0;
        while (x < (int)ft_strlen(game->map->grid[y]))
        {
            char c = game->map->grid[y][x];
            
            /* 0 veya oyuncu pozisyonu ise etrafını kontrol et */
            if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
            {
                if (check_surrounded(game, x, y) == -1)
                {
                    ft_putstr_fd("Error\nMap is not closed/surrounded by walls\n", 2);
                    ft_putstr_fd("Problem at position (", 2);
                    /* Basit integer yazdırma */
                    char buf[12];
                    int i = 0;
                    int tmp = x;
                    if (tmp == 0)
                        buf[i++] = '0';
                    while (tmp > 0)
                    {
                        buf[i++] = '0' + (tmp % 10);
                        tmp /= 10;
                    }
                    buf[i] = '\0';
                    /* Ters çevir */
                    int j = 0;
                    while (j < i / 2)
                    {
                        char t = buf[j];
                        buf[j] = buf[i - 1 - j];
                        buf[i - 1 - j] = t;
                        j++;
                    }
                    ft_putstr_fd(buf, 2);
                    ft_putstr_fd(", ", 2);
                    
                    i = 0;
                    tmp = y;
                    if (tmp == 0)
                        buf[i++] = '0';
                    while (tmp > 0)
                    {
                        buf[i++] = '0' + (tmp % 10);
                        tmp /= 10;
                    }
                    buf[i] = '\0';
                    j = 0;
                    while (j < i / 2)
                    {
                        char t = buf[j];
                        buf[j] = buf[i - 1 - j];
                        buf[i - 1 - j] = t;
                        j++;
                    }
                    ft_putstr_fd(buf, 2);
                    ft_putstr_fd(")\n", 2);
                    return (-1);
                }
            }
            x++;
        }
        y++;
    }
    
    return (0);
}

/* Tüm validasyonları çalıştır */
int validate_map(t_game *game)
{
    if (validate_map_chars(game) == -1)
        return (-1);
    if (validate_map_walls(game) == -1)
        return (-1);
    if (validate_map_closed(game) == -1)
        return (-1);
    return (0);
}