#ifndef CUB3D_H
# define CUB3D_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include <string.h>
# include <errno.h>
# include "../minilibx-linux/mlx.h"
# include "../libft/libft.h"

# define WIN_WIDTH 2048
# define WIN_HEIGHT 1024
# define ESC_KEY 65307
# define W_KEY 119
# define A_KEY 97
# define S_KEY 115
# define D_KEY 100
# define LEFT_ARROW 65361
# define RIGHT_ARROW 65363

typedef struct s_player
{
    double x;
    double y;
    double dir_x;
    double dir_y;
    double plane_x;
    double plane_y;
}   t_player;

typedef struct s_map
{
    char    **grid;
    int     width;
    int     height;
    char    *no_texture;
    char    *so_texture;
    char    *we_texture;
    char    *ea_texture;
    int     floor_color;
    int     ceiling_color;
}   t_map;

typedef struct s_game
{
    void        *mlx;
    void        *win;
    void        *img;
    char        *addr;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
    t_map       *map;
    t_player    *player;
}   t_game;

t_game  *init_game(void);
int     parse_map(t_game *game, char *filename);
int     init_graphics(t_game *game);
void    game_loop(t_game *game);
void    cleanup_game(t_game *game);
void    render_map(t_game *game);
char	*get_next_line(int fd);


#endif