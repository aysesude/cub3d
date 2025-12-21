NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

MLX_DIR = minilibx-linux
MLX = $(MLX_DIR)/libmlx_Linux.a

SRCS = main.c \
       src/init_game.c \
       src/utils.c \
       src/graphics.c \
       src/input.c \
       src/movement.c \
       src/movement2.c \
       src/draw_3d_map.c \
       src/texture.c \
       src/raycasting.c \
       src/parser/parse_identifiers.c \
       src/parser/parse_reader.c \
       src/parser/parse_texture.c \
       src/parser/parse_color.c \
       src/parser/parse_player.c \
       src/parser/parse_map_section.c \
       src/parser/parse_map_grid.c \
       src/parser/parse_validation.c \
       src/validation/map_validation.c \
       src/validation/map_validation_walls.c \
       src/validation/map_validation_closed.c \
       src/validation/map_wall_checker.c \
       get_next_line/get_next_line.c \
       get_next_line/get_next_line_utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX) -lXext -lX11 -lm -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(MLX):
	make -C $(MLX_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean
	make -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re