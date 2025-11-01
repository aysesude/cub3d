#include "../include/cub3d.h"

#define COLOR_WALL 0x555555
#define COLOR_PLAYER 0x00FF00
#define COLOR_BG 0x222222
#define COLOR_RAY 0xAAAAAA
#define COLOR_HIT 0xFF0000

void put_pixel(t_game *game, int x, int y, int color)
{
    char *dst;
    if (x < 0 || y < 0 || x >= WIN_WIDTH || y >= WIN_HEIGHT)
        return;
    dst = game->addr + (y * game->line_length + x * (game->bits_per_pixel / 8));
    *(unsigned int *)dst = color;
}

void draw_square(t_game *game, int x, int y, int size, int color)
{
    int i = 0;
    while (i < size)
    {
        int j = 0;
        while (j < size)
        {
            put_pixel(game, x + j, y + i, color);
            j++;
        }
        i++;
    }
}

void draw_circle(t_game *game, int cx, int cy, int r, int color)
{
    int y = -r;
    while (y <= r)
    {
        int x = -r;
        while (x <= r)
        {
            if (x * x + y * y <= r * r)
                put_pixel(game, cx + x, cy + y, color);
            x++;
        }
        y++;
    }
}

/* Işınları çizme */
void draw_line(t_game *game, int x0, int y0, int x1, int y1, int color)
{
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while (1)
    {
        put_pixel(game, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

double deg_to_rad(double deg)
{
    return deg * (M_PI / 180.0);
}

void render_map(t_game *game)
{
    int map_w = game->map->width;
    int map_h = game->map->height;

    /* KARE boyutu hesapla */
    int tile_size = WIN_WIDTH / map_w;
    if (WIN_HEIGHT / map_h < tile_size)
        tile_size = WIN_HEIGHT / map_h;

    int map_px_w = map_w * tile_size;
    int map_px_h = map_h * tile_size;
    int offset_x = (WIN_WIDTH - map_px_w) / 2;
    int offset_y = (WIN_HEIGHT - map_px_h) / 2;

    /* Arka plan */
    int y = 0;
    while (y < WIN_HEIGHT)
    {
        int x = 0;
        while (x < WIN_WIDTH)
        {
            put_pixel(game, x, y, COLOR_BG);
            x++;
        }
        y++;
    }

    /* Duvarlar */
    y = 0;
    while (y < map_h)
    {
        int x = 0;
        while (x < (int)ft_strlen(game->map->grid[y]))
        {
            int sx = offset_x + x * tile_size;
            int sy = offset_y + y * tile_size;
            if (game->map->grid[y][x] == '1')
                draw_square(game, sx, sy, tile_size, COLOR_WALL);
            x++;
        }
        y++;
    }

    /* Oyuncu ekran koordinatları */
    int px = offset_x + (int)(game->player->x * tile_size);
    int py = offset_y + (int)(game->player->y * tile_size);
    draw_circle(game, px, py, tile_size / 4, COLOR_PLAYER);

    /* ---------- RAYCASTING (2D DDA) ---------- */
    double fov = 66.0;
    double half = fov / 2.0;
    double step = 10.0;

    double player_angle = atan2(game->player->dir_y, game->player->dir_x);

    double off = -half;
    while (off <= half + 0.0001)
    {
        double ray_angle = player_angle + deg_to_rad(off);
        double ray_dir_x = cos(ray_angle);
        double ray_dir_y = sin(ray_angle);

        int mapX = (int)game->player->x;
        int mapY = (int)game->player->y;

        double sideDistX, sideDistY;
        double deltaDistX = (ray_dir_x == 0) ? 1e30 : fabs(1.0 / ray_dir_x);
        double deltaDistY = (ray_dir_y == 0) ? 1e30 : fabs(1.0 / ray_dir_y);
        int stepX, stepY;
        int hit = 0;
        int side = 0;

        if (ray_dir_x < 0)
        {
            stepX = -1;
            sideDistX = (game->player->x - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - game->player->x) * deltaDistX;
        }
        if (ray_dir_y < 0)
        {
            stepY = -1;
            sideDistY = (game->player->y - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - game->player->y) * deltaDistY;
        }

        int guard = 0;
        while (!hit && guard < 1000)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if (mapX < 0 || mapX >= map_w || mapY < 0 || mapY >= map_h)
                break;
            if (mapX < (int)ft_strlen(game->map->grid[mapY]) &&
                game->map->grid[mapY][mapX] == '1')
            {
                hit = 1;
                break;
            }
            guard++;
        }

        double hitX, hitY;
        if (hit)
        {
            double perpDist;
            if (side == 0)
                perpDist = (mapX - game->player->x + (1 - stepX) / 2.0) / ray_dir_x;
            else
                perpDist = (mapY - game->player->y + (1 - stepY) / 2.0) / ray_dir_y;

            hitX = game->player->x + perpDist * ray_dir_x;
            hitY = game->player->y + perpDist * ray_dir_y;
        }
        else
        {
            double maxd = 30.0;
            hitX = game->player->x + ray_dir_x * maxd;
            hitY = game->player->y + ray_dir_y * maxd;
        }

        int hit_px = offset_x + (int)(hitX * tile_size);
        int hit_py = offset_y + (int)(hitY * tile_size);

        draw_line(game, px, py, hit_px, hit_py, COLOR_RAY);

        if (hit)
            draw_circle(game, hit_px, hit_py, tile_size / 8 + 1, COLOR_HIT);

        off += step;
    }

    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}
