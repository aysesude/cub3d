/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 11:29:43 by aycami            #+#    #+#             */
/*   Updated: 2025/12/07 11:31:02 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# define WIN_WIDTH 2048
# define WIN_HEIGHT 1024

typedef struct s_player
{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;

typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
	char	*no_texture;
	char	*so_texture;
	char	*we_texture;
	char	*ea_texture;
	char	*do_texture;
	int		floor_color;
	int		ceiling_color;
}	t_map;

typedef struct s_texture
{
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_texture;

typedef struct s_ray
{
	double	ray_dir_x;
	double	ray_dir_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
	double	perp_wall_dist;
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
}	t_ray;

typedef struct s_keys
{
	int	w;
	int	s;
	int	a;
	int	d;
	int	left;
	int	right;
}	t_keys;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	t_map		*map;
	t_player	*player;
	t_texture	tex_no;
	t_texture	tex_so;
	t_texture	tex_we;
	t_texture	tex_ea;
	t_texture	tex_do;
	t_keys		keys;
}	t_game;

t_game	*init_game(void);
void	cleanup_game(t_game *game);
int		parse_map(t_game *game, char *filename);
char	*read_line(int fd);
int		is_map_line(char *line);
int		check_file_extension(char *filename);
int		validate_config(t_game *game);
int		parse_north_texture(t_game *game, char *line);
int		parse_south_texture(t_game *game, char *line);
int		parse_west_texture(t_game *game, char *line);
int		parse_east_texture(t_game *game, char *line);
int		parse_door_texture(t_game *game, char *line);
int		parse_rgb(char *str);
int		parse_floor_color(t_game *game, char *line);
int		parse_ceiling_color(t_game *game, char *line);
int		is_player_char(char c);
int		find_player(t_game *game);
int		is_valid_map_char(char c);
int		validate_map_chars(t_game *game);
char	get_map_char(t_game *game, int x, int y);
int		is_walkable(char c);
int		validate_map(t_game *game);
int		validate_map_walls(t_game *game);
int	validate_map_doors(t_game *game);
void	write_number(int n);
int		validate_map_closed(t_game *game);
int		init_graphics(t_game *game);
void	game_loop(t_game *game);
void	render_mini_map(t_game *game);
void	put_pixel(t_game *game, int x, int y, int color);
void	draw_line(t_game *game, int x0, int y0, int x1, int y1, int color);
void	draw_circle(t_game *game, int cx, int cy, int r, int color);
double	deg_to_rad(double deg);
char	*get_next_line(int fd);
void	render_3d(t_game *game);
int		load_textures(t_game *game);
int		get_texture_color(t_texture *tex, int x, int y);
void	init_ray(t_game *game, t_ray *ray, int x);
void	calc_step_and_side_dist(t_game *game, t_ray *ray);
void	perform_dda(t_game *game, t_ray *ray);
void	calc_wall_height(t_game *game, t_ray *ray);
t_texture	*get_wall_texture(t_game *game, t_ray *ray);
int		can_move(t_game *game, double x, double y);
void	move_forward(t_game *game);
void	move_backward(t_game *game);
void	move_left(t_game *game);
void	move_right(t_game *game);
void	rotate_left(t_game *game);
void	rotate_right(t_game *game);
void	handle_movement(t_game *game);
int		key_release(int keycode, t_game *game);
int		key_press(int keycode, t_game *game);

#endif