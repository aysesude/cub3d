NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

MLX_DIR = minilibx-linux
MLX = $(MLX_DIR)/libmlx_Linux.a

HEADER = include/cub3d_bonus.h

SRCS = main_bonus.c \
       src/init_game_bonus.c \
       src/utils_bonus.c \
       src/graphics_bonus.c \
       src/door_handler_bonus.c \
       src/door_texture_bonus.c \
       src/input_bonus.c \
       src/movement_bonus.c \
       src/movement2_bonus.c \
       src/draw_minimap_utils_bonus.c \
       src/draw_minimap_bonus.c \
       src/draw_minimap_rays_bonus.c \
       src/draw_minimap_rays_utils_bonus.c \
       src/draw_3d_map_bonus.c \
       src/texture_bonus.c \
       src/raycasting_bonus.c \
       src/parser/parse_file_bonus.c \
       src/parser/parse_utils_bonus.c \
       src/parser/parse_texture_bonus.c \
       src/parser/parse_texture2_bonus.c \
       src/parser/parse_map_grid_bonus.c \
       src/parser/parse_color_bonus.c \
       src/parser/parse_player_bonus.c \
       src/validation/map_validation_bonus.c \
       src/validation/map_validation_walls_bonus.c \
       src/validation/map_validation_doors_bonus.c \
       src/validation/map_validation_closed_bonus.c \
       get_next_line/get_next_line.c \
       get_next_line/get_next_line_utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX) -lXext -lX11 -lm -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(MLX):
	@make -C $(MLX_DIR)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean
	@make -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re