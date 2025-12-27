/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_doors.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 00:00:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/13 00:00:00 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d_bonus.h"

static int is_border_or_outside(t_game *game, int x, int y)
{
    // Kenar satır/sütun ve kırık satırlar (kısa satır) için güvenli kontrol
    if (y <= 0 || y >= game->map->height - 1)
        return (1);
    // Satır uzunluğu değişebileceği için get_map_char ile güvenli komşu erişimi yapacağız
    if (x <= 0)
        return (1);
    // Mevcut satırda sağ kenar kontrolü
    if (x >= (int)ft_strlen(game->map->grid[y]) - 1)
        return (1);
    return (0);
}

int validate_map_doors(t_game *game)
{
    int y;
    int x;

    y = 0;
    while (y < game->map->height)
    {
        x = 0;
        while (x < (int)ft_strlen(game->map->grid[y]))
        {
            if (game->map->grid[y][x] == 'D')
            {
                char n = get_map_char(game, x, y - 1);
                char s = get_map_char(game, x, y + 1);
                char w = get_map_char(game, x - 1, y);
                char e = get_map_char(game, x + 1, y);

                // Kapı mapin en dış kenarında olamaz (veya kırık satır nedeniyle dışa taşma)
                if (is_border_or_outside(game, x, y))
                {
                    ft_putstr_fd("Error\nDoor on map border\n", 2);
                    return (-1);
                }
                // Komşular güvenli (kırık satır) mı?
                if (n == '\0' || s == '\0' || w == '\0' || e == '\0' ||
                    n == ' ' || s == ' ' || w == ' ' || e == ' ')
                {
                    ft_putstr_fd("Error\nDoor adjacent to outside or space\n", 2);
                    return (-1);
                }
                // Yalnızca NS veya EW hizası: kapı iki karşı duvar arasında olmalı
                // NS durumunda: kuzey ve güney '1', doğu ve batı yürünebilir (zemin veya oyuncu)
                // EW durumunda: doğu ve batı '1', kuzey ve güney yürünebilir (zemin veya oyuncu)
                if ((n == '1' && s == '1'))
                {
                    if (!(is_walkable(w)) || !(is_walkable(e)))
                    {
                        ft_putstr_fd("Error\nDoor NS must have walkable east/west\n", 2);
                        return (-1);
                    }
                }
                else if ((w == '1' && e == '1'))
                {
                    if (!(is_walkable(n)) || !(is_walkable(s)))
                    {
                        ft_putstr_fd("Error\nDoor EW must have walkable north/south\n", 2);
                        return (-1);
                    }
                }
                else
                {
                    ft_putstr_fd("Error\nInvalid door orientation\n", 2);
                    return (-1);
                }
            }
            x++;
        }
        y++;
    }
    return (0);
}
