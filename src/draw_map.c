#include "../include/cub3d.h"

#define COLOR_WALL 0x555555
#define COLOR_PLAYER 0xFF0000
#define COLOR_BG 0x222222

static void put_pixel(t_game *game, int x, int y, int color)
{
    char *dst;
    if (x < 0 || y < 0 || x >= WIN_WIDTH || y >= WIN_HEIGHT)
        return;
    dst = game->addr + (y * game->line_length + x * (game->bits_per_pixel / 8));
    *(unsigned int *)dst = color;
}

static void draw_square(t_game *game, int x, int y, int size, int color)
{
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            put_pixel(game, x + j, y + i, color);
}

static void draw_circle(t_game *game, int cx, int cy, int r, int color)
{
    for (int y = -r; y <= r; y++)
        for (int x = -r; x <= r; x++)
            if (x * x + y * y <= r * r)
                put_pixel(game, cx + x, cy + y, color);
}

void render_map(t_game *game)
{
    int map_w = game->map->width;
    int map_h = game->map->height;

    // KARE boyutu hesapla
    int tile_size = WIN_WIDTH / map_w;
    if (WIN_HEIGHT / map_h < tile_size)
        tile_size = WIN_HEIGHT / map_h;

    // Map’in toplam piksel boyutları
    int map_px_w = map_w * tile_size;
    int map_px_h = map_h * tile_size;

    // Ortalamak için ofset hesapla
    int offset_x = (WIN_WIDTH - map_px_w) / 2;
    int offset_y = (WIN_HEIGHT - map_px_h) / 2;

    // Arka plan
    for (int y = 0; y < WIN_HEIGHT; y++)
        for (int x = 0; x < WIN_WIDTH; x++)
            put_pixel(game, x, y, COLOR_BG);

    // Kareleri çiz
    for (int y = 0; y < map_h; y++)
    {
        for (int x = 0; x < (int)ft_strlen(game->map->grid[y]); x++)
        {
            int sx = offset_x + x * tile_size;
            int sy = offset_y + y * tile_size;
            if (game->map->grid[y][x] == '1')
                draw_square(game, sx, sy, tile_size, COLOR_WALL);
        }
    }

    // Oyuncu
    int px = offset_x + (int)(game->player->x * tile_size);
    int py = offset_y + (int)(game->player->y * tile_size);
    draw_circle(game, px, py, tile_size / 4, COLOR_PLAYER);

    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}
