/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blender.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/30 14:27:06 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb			mlx_pixel_to_rgb(t_mlx *mlx, int x, int y);
static inline unsigned int	rgb_to_uint(t_rgb color);
/* -------------------------------------------------------------------------- */

void	blend_colors(t_minirt *mrt, t_ray *ray, t_vector2 pos)
{
	int		count;
	t_mlx	*mlx;
	t_rgb	prev_color;

	mlx = mrt->mlx;
	count = mlx->count;
	prev_color = mlx_pixel_to_rgb(mlx, pos.x, pos.y);
	*((unsigned int *)(mlx->data + (pos.y * mlx->ll + pos.x * mlx->cl)))
		= rgb_to_uint((t_rgb){
			(unsigned char)((prev_color.r * count + ray->color.r)
				/ (count + 1)),
			(unsigned char)((prev_color.g * count + ray->color.g)
				/ (count + 1)),
			(unsigned char)((prev_color.b * count + ray->color.b)
				/ (count + 1))});
}

static inline t_rgb	mlx_pixel_to_rgb(t_mlx *mlx, int x, int y)
{
	unsigned int	pixel;

	pixel = *((unsigned int *)(mlx->data + (y * mlx->ll + x * mlx->cl)));
	return ((t_rgb){(pixel >> 16) & 0xFF, (pixel >> 8) & 0xFF, pixel & 0xFF});
}

static inline unsigned int	rgb_to_uint(t_rgb color)
{
	return ((color.r & 0xFF) << 16
		| ((color.g & 0xFF) << 8)
		| (color.b & 0xFF));
}

t_rgb	apply_lights_modifier(t_frgb modifier, t_rgb base)
{
	base.r = (unsigned char) fmin(powf(powf((float)base.r / 255.0f, GAMMA)
				* fmin(modifier.r, 1.0f), 1.0f / GAMMA) * 255.0f, 255.0f);
	base.g = (unsigned char) fmin(powf(powf((float)base.g / 255.0f, GAMMA)
				* fmin(modifier.g, 1.0f), 1.0f / GAMMA) * 255.0f, 255.0f);
	base.b = (unsigned char) fmin(powf(powf((float)base.b / 255.0f, GAMMA)
				* fmin(modifier.b, 1.0f), 1.0f / GAMMA) * 255.0f, 255.0f);
	return (base);
}

void	apply_selection_effect(t_rgb *color)
{
	color->r += (0.2f * ((255 - color->r) - color->r));
	color->g += (0.2f * ((255 - color->g) - color->g));
	color->b += (0.2f * ((255 - color->b) - color->b));
}
