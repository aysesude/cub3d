#include "../include/cub3d.h"

int key_press(int keycode, t_game *game)
{
    if (keycode == ESC_KEY)
    {
        cleanup_game(game);
        exit(0);
    }
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
