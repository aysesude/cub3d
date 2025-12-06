#include "../include/cub3d.h"

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


void render_3d(t_game *game)
{
    game->map->floor_color = 0x444444;
    game->map->ceiling_color = 0x87CEEB; //şimdilik koydum daha sonra dosyadan alıncak

    int x;
    double posX = game->player->x;
    double posY = game->player->y;

    /* Her ekran sütunu için bir ray at */
    x = 0;
    while (x < WIN_WIDTH)
    {
        /* 1. Kamera X koordinatı (-1 ... 1) */
        double cameraX = 2 * x / (double)WIN_WIDTH - 1;

        /* 2. Ray yönü */
        double rayDirX = game->player->dir_x + game->player->plane_x * cameraX;
        double rayDirY = game->player->dir_y + game->player->plane_y * cameraX;

        /* 3. DDA için map hücreleri */
        int mapX = (int)posX;
        int mapY = (int)posY;

        /* 4. Ray adım uzunlukları */
        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1.0 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1.0 / rayDirY);

        double sideDistX;
        double sideDistY;
        int stepX, stepY;
        int hit = 0;
        int side = 0;

        /* 5. İlk adım mesafeleri */
        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }

        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        /* 6. DDA — duvar bulma */
        while (hit == 0)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0; // X eksenine çarptı
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1; // Y eksenine çarptı
            }

            if (game->map->grid[mapY][mapX] == '1')
                hit = 1;
        }

        /* 7. Perpendicular distance hesaplama */
        double perpWallDist;
        if (side == 0)
            perpWallDist = (mapX - posX + (1 - stepX) / 2.0) / rayDirX;
        else
            perpWallDist = (mapY - posY + (1 - stepY) / 2.0) / rayDirY;

        /* 8. Duvar yüksekliği */
        int lineHeight = (int)(WIN_HEIGHT / perpWallDist);

        /* 9. Çizim başlangıç & bitiş */
        int drawStart = -lineHeight / 2 + WIN_HEIGHT / 2;
        if (drawStart < 0)
            drawStart = 0;

        int drawEnd = lineHeight / 2 + WIN_HEIGHT / 2;
        if (drawEnd >= WIN_HEIGHT)
            drawEnd = WIN_HEIGHT - 1;

        /* 10. Tavan çizimi */
        int y = 0;
        while (y < drawStart)
        {
            put_pixel(game, x, y, game->map->ceiling_color);
            y++;
        }

        /* 11. Duvar çizimi - X/Y eksenine göre gölgelendirme */
        int wallColor = (side == 1) ? 0x888888 : 0xBBBBBB;
        draw_vertical_line(game, x, drawStart, drawEnd, wallColor);

        /* 12. Zemin çizimi */
        y = drawEnd + 1;
        while (y < WIN_HEIGHT)
        {
            put_pixel(game, x, y, game->map->floor_color);
            y++;
        }

        x++;
    }

    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

