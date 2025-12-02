#include "../include/cub3d.h"
#include <math.h>

#define MOVE_SPEED 0.15
#define ROT_SPEED 0.13962634  // 8 derece (8 * PI / 180)

static int is_wall(t_game *game, double x, double y)
{
    int map_x = (int)x;
    int map_y = (int)y;

    if (map_x < 0 || map_y < 0 || map_y >= game->map->height)
        return (1);
    if (map_x >= (int)ft_strlen(game->map->grid[map_y]))
        return (1);
    return (game->map->grid[map_y][map_x] == '1');
}

static void move_forward(t_game *game)
{
    double new_x = game->player->x + game->player->dir_x * MOVE_SPEED;
    double new_y = game->player->y + game->player->dir_y * MOVE_SPEED;

    if (!is_wall(game, new_x, game->player->y))
        game->player->x = new_x;
    if (!is_wall(game, game->player->x, new_y))
        game->player->y = new_y;
}

static void move_backward(t_game *game)
{
    double new_x = game->player->x - game->player->dir_x * MOVE_SPEED;
    double new_y = game->player->y - game->player->dir_y * MOVE_SPEED;

    if (!is_wall(game, new_x, game->player->y))
        game->player->x = new_x;
    if (!is_wall(game, game->player->x, new_y))
        game->player->y = new_y;
}

static void move_right(t_game *game)
{
    double new_x = game->player->x - game->player->dir_y * MOVE_SPEED;
    double new_y = game->player->y + game->player->dir_x * MOVE_SPEED;

    if (!is_wall(game, new_x, game->player->y))
        game->player->x = new_x;
    if (!is_wall(game, game->player->x, new_y))
        game->player->y = new_y;
}

static void move_left(t_game *game)
{
    double new_x = game->player->x + game->player->dir_y * MOVE_SPEED;
    double new_y = game->player->y - game->player->dir_x * MOVE_SPEED;

    if (!is_wall(game, new_x, game->player->y))
        game->player->x = new_x;
    if (!is_wall(game, game->player->x, new_y))
        game->player->y = new_y;
}

static void rotate_left(t_game *game)
{
    double old_dir_x = game->player->dir_x;
    double old_plane_x = game->player->plane_x;

    game->player->dir_x = game->player->dir_x * cos(ROT_SPEED) - game->player->dir_y * sin(ROT_SPEED);
    game->player->dir_y = old_dir_x * sin(ROT_SPEED) + game->player->dir_y * cos(ROT_SPEED);
    game->player->plane_x = game->player->plane_x * cos(ROT_SPEED) - game->player->plane_y * sin(ROT_SPEED);
    game->player->plane_y = old_plane_x * sin(ROT_SPEED) + game->player->plane_y * cos(ROT_SPEED);
}

static void rotate_right(t_game *game)
{
    double old_dir_x = game->player->dir_x;
    double old_plane_x = game->player->plane_x;

    game->player->dir_x = game->player->dir_x * cos(-ROT_SPEED) - game->player->dir_y * sin(-ROT_SPEED);
    game->player->dir_y = old_dir_x * sin(-ROT_SPEED) + game->player->dir_y * cos(-ROT_SPEED);
    game->player->plane_x = game->player->plane_x * cos(-ROT_SPEED) - game->player->plane_y * sin(-ROT_SPEED);
    game->player->plane_y = old_plane_x * sin(-ROT_SPEED) + game->player->plane_y * cos(-ROT_SPEED);
}

int key_press(int keycode, t_game *game)
{
    if (keycode == ESC_KEY)
    {
        cleanup_game(game);
        exit(0);
    }
    else if (keycode == 119)
        move_forward(game);
    else if (keycode == 115)
        move_backward(game);
    else if (keycode == 97)
        move_left(game);
    else if (keycode == 100)
        move_right(game);
    else if (keycode == 65363)
        rotate_left(game);
    else if (keycode == 65361)
        rotate_right(game);
    return (0);
}

int close_window(t_game *game)
{
    cleanup_game(game);
    exit(0);
    return (0);
}

int init_graphics(t_game *game)
{
    game->mlx = mlx_init();
    if (!game->mlx)
    {
        ft_putstr_fd("Error\nMLX initialization failed\n", 2);
        return (-1);
    }
    game->win = mlx_new_window(game->mlx, WIN_WIDTH, WIN_HEIGHT, "cub3D");
    if (!game->win)
    {
        ft_putstr_fd("Error\nWindow creation failed\n", 2);
        return (-1);
    }
    game->img = mlx_new_image(game->mlx, WIN_WIDTH, WIN_HEIGHT);
    if (!game->img)
    {
        ft_putstr_fd("Error\nImage creation failed\n", 2);
        return (-1);
    }
    game->addr = mlx_get_data_addr(game->img, &game->bits_per_pixel,
                                   &game->line_length, &game->endian);
    
    mlx_hook(game->win, 2, 1L<<0, key_press, game);
    mlx_hook(game->win, 17, 0, close_window, game);
    return (0);
}

static int render_frame(t_game *game)
{
    render_map(game);
    return (0);
}

void game_loop(t_game *game)
{
    mlx_loop_hook(game->mlx, render_frame, game);
    mlx_loop(game->mlx);
}
