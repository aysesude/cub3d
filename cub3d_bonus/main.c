/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap_rays.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by aycami            #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/cub3d_bonus.h"

/*
** init_and_validate - Oyun dosyasini ayriştirir ve dogrular
** @game: Oyun yapisi
** @filename: .cub dosyasinin yolu
**
** Bu fonksiyon üç ana işlem yapar:
** 1. parse_map: .cub dosyasini okur ve harita/texture bilgilerini ayirar
** 2. validate_map: Haritanin gecerli olup olmadigini kontrol eder
** 3. init_graphics: MLX grafik sistemini baslatir
**
** Basarili ise 0, hata durumunda -1 döner
*/
static int	init_and_validate(t_game *game, char *filename)
{
	if (parse_map(game, filename) == -1)
	{
		get_next_line(-1);
		return (-1);
	}
	if (validate_map(game) == -1)
	{
		get_next_line(-1);
		return (-1);
	}
	if (init_graphics(game) == -1)
	{
		get_next_line(-1);
		return (-1);
	}
	return (0);
}

/*
** main - Program giris noktasi
** @argc: Argüman sayisi
** @argv: Argüman dizisi
**
** Kullanim: ./cub3D <map.cub>
**
** Program akisi:
** 1. Argüman sayisini kontrol et (tam 2 olmali)
** 2. init_game ile oyun yapisini olustur ve sifirla
** 3. init_and_validate ile haritayi yukle ve grafikleri baslat
** 4. game_loop ile oyun döngüsünü baslat (MLX loop)
** 5. cleanup_game ile bellegi temizle
**
** Basarili ise 0, hata durumunda 1 döner
*/
int	main(int argc, char **argv)
{
	t_game	*game;

	if (argc != 2)
	{
		ft_putstr_fd("Error\nUsage: ./cub3D <map.cub>\n", 2);
		return (1);
	}
	game = init_game();
	if (!game)
	{
		ft_putstr_fd("Error\nMemory allocation failed\n", 2);
		return (1);
	}
	if (init_and_validate(game, argv[1]) == -1)
	{
		cleanup_game(game);
		return (1);
	}
	game_loop(game);
	cleanup_game(game);
	return (0);
}
