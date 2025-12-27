/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 03:11:50 by aycami            #+#    #+#             */
/*   Updated: 2025/12/14 17:03:59 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

/*
** close_window - Pencere kapatildiginda çagrilir
** @game: Oyun yapisi
**
** X butonuna basinca veya ESC tusuna basinca tetiklenir
** cleanup_game ile tüm bellegi temizler ve programi sonlandirir
**
** Her zaman 0 döner (MLX hook gereksinimleri için)
*/
/*
** close_window - Pencere kapatıldığında çağrılır
** @game: Oyun yapısı
**
** X butonuna basınca veya ESC tuşuna basınca tetiklenir
** cleanup_game ile tüm belleği temizler ve programı sonlandırır
**
** Her zaman 0 döner (MLX hook gereksinimleri için)
*/
int	close_window(t_game *game)
{
	cleanup_game(game);
	exit(0);
	return (0);
}

/*
** init_graphics - MLX grafik sistemini baslatir
** @game: Oyun yapisi
**
** Grafik altyapisini kurar:
** 1. mlx_init: X11 baglantisi olustur
** 2. mlx_new_window: WIN_WIDTH x WIN_HEIGHT boyutunda pencere aç
** 3. mlx_new_image: Çizim için arka tampon (double buffering)
** 4. mlx_get_data_addr: Piksel verisine erisim için adres al
** 5. load_textures: XPM texture dosyalarini yükle
** 6. mlx_hook: Klavye ve pencere olaylarini bagla
**
** Hook olaylari:
** - 2: KeyPress, 3: KeyRelease, 17: DestroyNotify (pencere kapat)
**
** Basarili ise 0, hata durumunda -1 döner
*/
/*
** init_graphics - MLX grafik sistemini başlatır
** @game: Oyun yapısı
**
** Grafik altyapısını kurar:
** 1. mlx_init: X11 bağlantısı oluştur
** 2. mlx_new_window: WIN_WIDTH x WIN_HEIGHT boyutunda pencere aç
** 3. mlx_new_image: Çizim için arka tampon (double buffering)
** 4. mlx_get_data_addr: Piksel verisine erişim için adres al
** 5. load_textures: XPM texture dosyalarını yükle
** 6. mlx_hook: Klavye ve pencere olaylarını bağla
**
** Hook olayları:
** - 2: KeyPress, 3: KeyRelease, 17: DestroyNotify (pencere kapat)
**
** Başarılı ise 0, hata durumunda -1 döner
*/
int	init_graphics(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (ft_putstr_fd("Error\nMLX initialization failed\n", 2), -1);
	game->win = mlx_new_window(game->mlx, WIN_WIDTH, WIN_HEIGHT, "cub3D");
	if (!game->win)
		return (ft_putstr_fd("Error\nWindow creation failed\n", 2), -1);
	game->img = mlx_new_image(game->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!game->img)
		return (ft_putstr_fd("Error\nImage creation failed\n", 2), -1);
	game->addr = mlx_get_data_addr(game->img, &game->bits_per_pixel,
			&game->line_length, &game->endian);
	if (!load_textures(game))
		return (ft_putstr_fd("Error\nTexture loading failed\n", 2), -1);
	mlx_hook(game->win, 2, 1L << 0, key_press, game);
	mlx_hook(game->win, 3, 1L << 1, key_release, game);
	mlx_hook(game->win, 17, 0, close_window, game);
	return (0);
}

/*
** render_frame - Her frame'de çagrilir, oyun durumunu günceller ve çizer
** @game: Oyun yapisi
**
** Her frame'de yapilan islemler:
** 1. handle_movement: Klavye girislerine göre oyuncuyu hareket ettir
** 2. handle_doors: Kapilari açip kapat (mesafeye göre)
** 3. render_3d: Raycasting ile 3D sahneyi çiz
** 4. render_mini_map: Sol alt köseye minimap çiz
**
** mlx_loop_hook tarafindan sürekli çagrilir
** Her zaman 0 döner
*/
/*
** render_frame - Her frame'de çağrılır, oyun durumunu günceller ve çizer
** @game: Oyun yapısı
**
** Her frame'de yapılan işlemler:
** 1. handle_movement: Klavye girişlerine göre oyuncuyu hareket ettir
** 2. handle_doors: Kapıları açıp kapat (mesafeye göre)
** 3. render_3d: Raycasting ile 3D sahneyi çiz
** 4. render_mini_map: Sol alt köşeye minimap çiz
**
** mlx_loop_hook tarafından sürekli çağrılır
** Her zaman 0 döner
*/
static int	render_frame(t_game *game)
{
	handle_movement(game);
	handle_doors(game);
	render_3d(game);
	render_mini_map(game);
	return (0);
}

/*
** game_loop - Ana oyun döngüsünü baslatir
** @game: Oyun yapisi
**
** mlx_loop_hook ile render_frame fonksiyonunu kayit eder
** mlx_loop sonsuz döngüye girer ve olaylari isler
**
** Döngü boyunca:
** - Olaylar (klavye, pencere) kontrol edilir
** - render_frame her iterasyonda çagrilir
** - Pencere kapaninca döngü sonlanir
*/
/*
** game_loop - Ana oyun döngüsünü başlatır
** @game: Oyun yapısı
**
** mlx_loop_hook ile render_frame fonksiyonunu kayıt eder
** mlx_loop sonsuz döngüye girer ve olayları işler
**
** Döngü boyunca:
** - Olaylar (klavye, pencere) kontrol edilir
** - render_frame her iterasyonda çağrılır
** - Pencere kapanınca döngü sonlanır
*/
void	game_loop(t_game *game)
{
	mlx_loop_hook(game->mlx, render_frame, game);
	mlx_loop(game->mlx);
}
