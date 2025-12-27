/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_3d_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 03:11:41 by aycami            #+#    #+#             */
/*   Updated: 2025/12/14 17:42:13 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

/*
** calc_tex_x - Duvar texture'ında hangi x koordinatının kullanılacağını hesaplar
** @ray: O anki ışın bilgisi
** @tex: Kullanılacak texture
** @player: Oyuncu bilgisi
**
** Duvarın vurulduğu noktayı (wall_x) bulur, texture genişliğiyle çarparak
** hangi sütunun çizileceğini belirler. Duvarın hangi yüzüne çarptığına göre
** texture'ın ters çevrilmesi gerekebilir.
**
** Döner: Texture'ın x koordinatı (int)
*/
static int	calc_tex_x(t_ray *ray, t_texture *tex, t_player *player)
{
	double	wall_x;
	int		tex_x;

	if (ray->side == 0)
		wall_x = player->y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		wall_x = player->x + ray->perp_wall_dist * ray->ray_dir_x;
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * (double)tex->width);
	if ((ray->side == 0 && ray->ray_dir_x < 0)
		|| (ray->side == 1 && ray->ray_dir_y > 0))
		tex_x = tex->width - tex_x - 1;
	return (tex_x);
}

/*
** draw_wall_texture - Bir duvar sütununu texture ile çizer
** @game: Oyun yapısı
** @ray: O anki ışın bilgisi
** @x: Ekranda çizilecek sütun (piksel)
** @tex: Kullanılacak duvar texture'ı
**
** Dikey olarak duvarın üstünden altına kadar (draw_start -> draw_end)
** texture'ın uygun y koordinatını hesaplar ve ekrana çizer.
*/
static void	draw_wall_texture(t_game *game, t_ray *ray, int x, t_texture *tex)
{
	int		y;
	int		tex_x;
	int		tex_y;
	double	step;
	double	tex_pos;

	tex_x = calc_tex_x(ray, tex, game->player);
	step = 1.0 * tex->height / ray->line_height;
	tex_pos = (ray->draw_start - WIN_HEIGHT / 2 + ray->line_height / 2) * step;
	y = ray->draw_start;
	while (y <= ray->draw_end)
	{
		tex_y = (int)tex_pos & (tex->height - 1);
		tex_pos += step;
		put_pixel(game, x, y, get_texture_color(tex, tex_x, tex_y));
		y++;
	}
}

/*
** draw_column - Ekrandaki bir sütunu (x) çizer: tavan, duvar, zemin
** @game: Oyun yapısı
** @ray: O anki ışın bilgisi
** @x: Ekranda çizilecek sütun (piksel)
**
** 1. draw_start'a kadar tavan rengini çizer
** 2. draw_wall_texture ile duvarı çizer (kapı varsa kapı texture'ı)
** 3. draw_end'den sonra zemin rengini çizer
*/
static void	draw_column(t_game *game, t_ray *ray, int x)
{
	int			y;
	t_texture	*tex;

	y = 0;
	while (y < ray->draw_start)
	{
		put_pixel(game, x, y, game->map->ceiling_color);
		y++;
	}
	tex = get_door_texture(game, ray);
	draw_wall_texture(game, ray, x, tex);
	y = ray->draw_end + 1;
	while (y < WIN_HEIGHT)
	{
		put_pixel(game, x, y, game->map->floor_color);
		y++;
	}
}

/*
** render_3d - 3D raycasting motorunun ana fonksiyonu (EN AYRINTILI AÇIKLAMA)
** @game: Oyun yapısı
**
** Ekranın her sütunu (x) için bir ışın gönderir ve duvarları çizer.
**
** 1. Döngü: x = 0'dan WIN_WIDTH'e kadar (her piksel sütun için)
**    a. init_ray: Işının yönünü ve harita üzerindeki başlangıç noktasını hesaplar
**    b. calc_step_and_side_dist: DDA algoritması için adım ve ilk mesafeleri ayarlar
**    c. perform_dda: DDA (Digital Differential Analyzer) ile ışın haritada ilerler,
**       ilk duvara veya kapıya çarpana kadar grid hücrelerinde adım atar
**    d. calc_wall_height: Duvara olan dik mesafeyi ve ekranda çizilecek yükseklikleri hesaplar
**    e. draw_column: O sütunu (tavan, duvar, zemin) çizer
**
** Raycasting algoritması:
** - Her sütun için oyuncunun bakış açısına göre bir ışın gönderilir
** - Işın harita gridinde DDA ile ilerler, duvar veya kapı bulana kadar
** - Duvara çarpınca mesafe hesaplanır, perspektif düzeltmesi yapılır
** - Texture mapping ile duvarın doğru kısmı ekrana çizilir
** - Kapı varsa get_door_texture ile kapı texture'ı kullanılır
**
** Son olarak, çizilen görüntü MLX ile pencereye basılır
*/
void	render_3d(t_game *game)
{
	int		x;
	t_ray	ray;

	game->map->floor_color = 0x444444;
	game->map->ceiling_color = 0x87ceeb;
	x = 0;
	while (x < WIN_WIDTH)
	{
		init_ray(game, &ray, x);
		calc_step_and_side_dist(game, &ray);
		perform_dda(game, &ray);
		calc_wall_height(game, &ray);
		draw_column(game, &ray, x);
		x++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}
