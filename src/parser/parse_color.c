#include "../../include/cub3d.h"

/* Stringden 1 RGB number parse et */
static int	parse_single_rgb(char *str, int *i)
{
	int	num;

	num = 0;
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
	if (str[*i] < '0' || str[*i] > '9')
		return (-1);
	while (str[*i] >= '0' && str[*i] <= '9')
	{
		num = num * 10 + (str[*i] - '0');
		(*i)++;
	}
	if (num < 0 || num > 255)
		return (-1);
	return (num);
}

/* RGB stringi color integer donustur */
static int	rgb_to_color(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}


int	parse_rgb(char *str)
{
	int	nums[3];
	int	i;
	int	idx;

	i = 0;
	idx = 0;
	while (idx < 3)
	{
		nums[idx] = parse_single_rgb(str, &i);
		if (nums[idx] == -1)
			return (-1);
		idx++;
		while (str[i] == ' ' || str[i] == '\t')
			i++;
		if (idx < 3 && str[i] == ',')
			i++;
		else if (idx < 3)
			return (-1);
	}
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] != '\0')
		return (-1);
	return (rgb_to_color(nums[0], nums[1], nums[2]));
}

int	parse_floor_color(t_game *game, char *line)
{
	int	color;

	if (game->map->floor_color != -1)
		return (ft_putstr_fd("Error\nDuplicate F color\n", 2), -1);
	color = parse_rgb(line + 1);
	if (color == -1)
		return (ft_putstr_fd("Error\nInvalid floor color format\n", 2), -1);
	game->map->floor_color = color;
	return (1);
}

int	parse_ceiling_color(t_game *game, char *line)
{
	int	color;

	if (game->map->ceiling_color != -1)
		return (ft_putstr_fd("Error\nDuplicate C color\n", 2), -1);
	color = parse_rgb(line + 1);
	if (color == -1)
		return (ft_putstr_fd("Error\nInvalid ceiling color format\n", 2), -1);
	game->map->ceiling_color = color;
	return (1);
}