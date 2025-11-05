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

/* Satırın başındaki ve sonundaki boşlukları temizle */
static char *trim_line(char *line)
{
    char *start = line;
    char *end;
    
    if (!line)
        return (NULL);
    
    /* Baştan boşlukları atla */
    while (*start == ' ' || *start == '\t')
        start++;
    
    /* Boş satır kontrolü */
    if (*start == '\0')
        return (start);
    
    /* Sondan boşlukları bul */
    end = start + ft_strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t'))
        end--;
    
    *(end + 1) = '\0';
    return (start);
}

/* RGB string'ini parse et: "220,100,0" -> color int */
static int parse_rgb(char *str)
{
    int nums[3] = {0, 0, 0};
    int idx = 0;
    int i = 0;
    
    while (str[i] && idx < 3)
    {
        /* Boşlukları atla */
        while (str[i] == ' ' || str[i] == '\t')
            i++;
        
        /* Sayıyı oku */
        if (str[i] >= '0' && str[i] <= '9')
        {
            nums[idx] = 0;
            while (str[i] >= '0' && str[i] <= '9')
            {
                nums[idx] = nums[idx] * 10 + (str[i] - '0');
                i++;
            }
            
            /* 0-255 kontrolü */
            if (nums[idx] < 0 || nums[idx] > 255)
                return (-1);
            
            idx++;
        }
        else if (str[i] != ',' && str[i] != '\0')
            return (-1); /* Geçersiz karakter */
        
        /* Virgül kontrolü */
        while (str[i] == ' ' || str[i] == '\t')
            i++;
        if (str[i] == ',')
            i++;
    }
    
    if (idx != 3)
        return (-1);
    
    return ((nums[0] << 16) | (nums[1] << 8) | nums[2]);
}

/* String'in map satırı olup olmadığını kontrol et */
static int is_map_line(char *line)
{
    int i = 0;
    int has_content = 0;
    
    while (line[i])
    {
        if (line[i] == '1' || line[i] == '0' || 
            line[i] == 'N' || line[i] == 'S' || 
            line[i] == 'E' || line[i] == 'W')
            has_content = 1;
        else if (line[i] != ' ' && line[i] != '\t')
            return (0); /* Geçersiz karakter */
        i++;
    }
    return (has_content);
}

/* Texture/Color identifier'ı parse et */
static int parse_identifier(t_game *game, char *line)
{
    char *trimmed = trim_line(line);
    char *path;
    int len;
    
    if (!trimmed || *trimmed == '\0')
        return (0); /* Boş satır, devam */
    
    len = ft_strlen(trimmed);
    
    /* North texture */
    if (len >= 3 && trimmed[0] == 'N' && trimmed[1] == 'O' && 
        (trimmed[2] == ' ' || trimmed[2] == '\t'))
    {
        path = trim_line(trimmed + 2);
        if (!path || *path == '\0')
            return (ft_putstr_fd("Error\nEmpty NO texture path\n", 2), -1);
        if (game->map->no_texture)
            return (ft_putstr_fd("Error\nDuplicate NO texture\n", 2), -1);
        game->map->no_texture = ft_strdup(path);
        return (1);
    }
    
    /* South texture */
    if (len >= 3 && trimmed[0] == 'S' && trimmed[1] == 'O' && 
        (trimmed[2] == ' ' || trimmed[2] == '\t'))
    {
        path = trim_line(trimmed + 2);
        if (!path || *path == '\0')
            return (ft_putstr_fd("Error\nEmpty SO texture path\n", 2), -1);
        if (game->map->so_texture)
            return (ft_putstr_fd("Error\nDuplicate SO texture\n", 2), -1);
        game->map->so_texture = ft_strdup(path);
        return (1);
    }
    
    /* West texture */
    if (len >= 3 && trimmed[0] == 'W' && trimmed[1] == 'E' && 
        (trimmed[2] == ' ' || trimmed[2] == '\t'))
    {
        path = trim_line(trimmed + 2);
        if (!path || *path == '\0')
            return (ft_putstr_fd("Error\nEmpty WE texture path\n", 2), -1);
        if (game->map->we_texture)
            return (ft_putstr_fd("Error\nDuplicate WE texture\n", 2), -1);
        game->map->we_texture = ft_strdup(path);
        return (1);
    }
    
    /* East texture */
    if (len >= 3 && trimmed[0] == 'E' && trimmed[1] == 'A' && 
        (trimmed[2] == ' ' || trimmed[2] == '\t'))
    {
        path = trim_line(trimmed + 2);
        if (!path || *path == '\0')
            return (ft_putstr_fd("Error\nEmpty EA texture path\n", 2), -1);
        if (game->map->ea_texture)
            return (ft_putstr_fd("Error\nDuplicate EA texture\n", 2), -1);
        game->map->ea_texture = ft_strdup(path);
        return (1);
    }
    
    /* Floor color */
    if (len >= 2 && trimmed[0] == 'F' && (trimmed[1] == ' ' || trimmed[1] == '\t'))
    {
        if (game->map->floor_color != -1)
            return (ft_putstr_fd("Error\nDuplicate F color\n", 2), -1);
        game->map->floor_color = parse_rgb(trimmed + 1);
        if (game->map->floor_color == -1)
            return (ft_putstr_fd("Error\nInvalid floor color format\n", 2), -1);
        return (1);
    }
    
    /* Ceiling color */
    if (len >= 2 && trimmed[0] == 'C' && (trimmed[1] == ' ' || trimmed[1] == '\t'))
    {
        if (game->map->ceiling_color != -1)
            return (ft_putstr_fd("Error\nDuplicate C color\n", 2), -1);
        game->map->ceiling_color = parse_rgb(trimmed + 1);
        if (game->map->ceiling_color == -1)
            return (ft_putstr_fd("Error\nInvalid ceiling color format\n", 2), -1);
        return (1);
    }
    
    /* Map başlangıcı kontrolü - daha kesin */
    if (is_map_line(trimmed))
        return (2); /* Map başladı */
    
    return (ft_putstr_fd("Error\nUnknown identifier or invalid line\n", 2), -1);
}

/* Tüm texture ve color'ların alındığını kontrol et */
static int validate_config(t_game *game)
{
    if (!game->map->no_texture)
        return (ft_putstr_fd("Error\nMissing NO texture\n", 2), -1);
    if (!game->map->so_texture)
        return (ft_putstr_fd("Error\nMissing SO texture\n", 2), -1);
    if (!game->map->we_texture)
        return (ft_putstr_fd("Error\nMissing WE texture\n", 2), -1);
    if (!game->map->ea_texture)
        return (ft_putstr_fd("Error\nMissing EA texture\n", 2), -1);
    if (game->map->floor_color == -1)
        return (ft_putstr_fd("Error\nMissing floor color\n", 2), -1);
    if (game->map->ceiling_color == -1)
        return (ft_putstr_fd("Error\nMissing ceiling color\n", 2), -1);
    return (0);
}

/* Oyuncu pozisyonunu bul ve yönünü ayarla */
static int find_player(t_game *game)
{
    int y = 0;
    int player_found = 0;
    
    while (y < game->map->height)
    {
        int x = 0;
        while (x < (int)ft_strlen(game->map->grid[y]))
        {
            char c = game->map->grid[y][x];
            if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
            {
                if (player_found)
                    return (ft_putstr_fd("Error\nMultiple players found\n", 2), -1);
                
                game->player->x = x + 0.5;
                game->player->y = y + 0.5;
                
                /* Yön vektörü */
                if (c == 'N') { game->player->dir_x = 0;  game->player->dir_y = -1; }
                if (c == 'S') { game->player->dir_x = 0;  game->player->dir_y = 1;  }
                if (c == 'E') { game->player->dir_x = 1;  game->player->dir_y = 0;  }
                if (c == 'W') { game->player->dir_x = -1; game->player->dir_y = 0;  }
                
                double scale = 0.66;
                game->player->plane_x = -game->player->dir_y * scale;
                game->player->plane_y = game->player->dir_x * scale;
                
                game->map->grid[y][x] = '0';
                player_found = 1;
            }
            x++;
        }
        y++;
    }
    
    if (!player_found)
        return (ft_putstr_fd("Error\nNo player found in map\n", 2), -1);
    
    return (0);
}

int parse_map(t_game *game, char *filename)
{
    int fd;
    char *line;
    char **grid = NULL;
    int rows = 0;
    int cols = 0;
    int map_started = 0;
    
    /* Dosya kontrolü */
    int len = 0;
    while (filename[len])
        len++;
    if (len < 4 || filename[len-4] != '.' || filename[len-3] != 'c' ||
        filename[len-2] != 'u' || filename[len-1] != 'b')
        return (ft_putstr_fd("Error\nFile must have .cub extension\n", 2), -1);
    
    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (ft_putstr_fd("Error\nFile open failed\n", 2), -1);
    
    /* Phase 1: Texture ve color parse */
    while ((line = read_line(fd)) && !map_started)
    {
        int result = parse_identifier(game, line);
        if (result == -1)
        {
            free(line);
            close(fd);
            return (-1);
        }
        if (result == 2)
        {
            map_started = 1;
            /* İlk map satırını kaydet */
            grid = malloc(sizeof(char *));
            grid[0] = ft_strdup(line);
            rows = 1;
            if ((int)ft_strlen(line) > cols)
                cols = ft_strlen(line);
        }
        free(line);
    }
    
    /* Config kontrolü */
    if (validate_config(game) == -1)
    {
        close(fd);
        return (-1);
    }
    
    /* Phase 2: Map parse (devam eden satırlar) */
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
    
    if (rows == 0)
        return (ft_putstr_fd("Error\nNo map found\n", 2), -1);
    
    game->map->grid = grid;
    game->map->height = rows;
    game->map->width = cols;
    
    /* Oyuncu bul */
    return find_player(game);
}