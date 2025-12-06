#include "../include/cub3d.h"

#define CEILING_COLOR 0x87CEEB   // Mavi gökyüzü
#define FLOOR_COLOR   0x444444   // Gri zemin
#define WALL_COLOR    0xAAAAAA   // Açık gri duvar sonra texture eklencek

void draw_vertical_line(t_game *game, int x, int start, int end, int color) //düz çizgi duvar için kullanıcam
{
    if (start < 0)
        start = 0;
    if (end >= WIN_HEIGHT)
        end = WIN_HEIGHT - 1;

    int y = start;
    while (y <= end)
    {
        put_pixel(game, x, y, color);
        y++;
    }
}

void render_simple_3d(t_game *game)
{
    int x, y;

    /* ----- 1. Gökyüzü boyama ----- */
    for (y = 0; y < WIN_HEIGHT / 2; y++)
    {
        for (x = 0; x < WIN_WIDTH; x++)
            put_pixel(game, x, y, CEILING_COLOR);
    }

    /* ----- 2. Zemin boyama ----- */
    for (y = WIN_HEIGHT / 2; y < WIN_HEIGHT; y++)
    {
        for (x = 0; x < WIN_WIDTH; x++)
            put_pixel(game, x, y, FLOOR_COLOR);
    }

    /* ----- 3. Ortada test duvarı ----- */
    int wallHeight = WIN_HEIGHT / 3;     // sabit bir yükseklik
    int start = WIN_HEIGHT / 2 - wallHeight / 2;
    int end   = WIN_HEIGHT / 2 + wallHeight / 2;

    int wall_x = WIN_WIDTH / 2;          // ekranın ortası
    draw_vertical_line(game, wall_x, start, end, WALL_COLOR);

    /* ----- 4. Ekrana bas ----- */
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}
