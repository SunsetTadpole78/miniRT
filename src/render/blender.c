/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blender.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/24 22:43:09 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline unsigned int	rgb_to_uint(t_rgb color);
/* -------------------------------------------------------------------------- */

void	blend_colors(t_minirt *mrt, t_ray *ray, t_vector2 pos, int count)
{
	t_mlx		*mlx;
	t_rgb		prev_color;
	t_mlx_image	image;

	mlx = mrt->mlx;
	image = mlx->image;
	prev_color = mlx_pixel_to_rgb(image, pos.x, pos.y);
	*((unsigned int *)(image.data + (pos.y * image.ll + pos.x * image.cl)))
		= rgb_to_uint((t_rgb){
			(unsigned char)((prev_color.r * count + ray->color.r)
				/ (count + 1)),
			(unsigned char)((prev_color.g * count + ray->color.g)
				/ (count + 1)),
			(unsigned char)((prev_color.b * count + ray->color.b)
				/ (count + 1))});
}

static inline unsigned int	rgb_to_uint(t_rgb color)
{
	return ((color.r & 0xFF) << 16
		| ((color.g & 0xFF) << 8)
		| (color.b & 0xFF));
}

t_rgb	apply_lights_modifier(t_frgb modifier, t_rgb base)
{
	float	gamma_inv;
	t_frgb	temp;

	gamma_inv = 1.0f / GAMMA;
	temp = (t_frgb){(float)base.r / 255.0f, (float)base.g / 255.0f,
		(float)base.b / 255.0f};
	if (modifier.r <= 1.0f)
		temp.r *= powf(modifier.r, gamma_inv);
	if (modifier.g <= 1.0f)
		temp.g *= powf(modifier.g, gamma_inv);
	if (modifier.b <= 1.0f)
		temp.b *= powf(modifier.b, gamma_inv);
	if (temp.r > 1.0f)
		base.r = 255;
	else
		base.r = (unsigned char)(temp.r * 255.0f);
	if (temp.g > 1.0f)
		base.g = 255;
	else
		base.g = (unsigned char)(temp.g * 255.0f);
	if (temp.b > 1.0f)
		base.b = 255;
	else
		base.b = (unsigned char)(temp.b * 255.0f);
	return (base);
}

void	apply_selection_effect(t_rgb *color)
{
	color->r += (0.2f * ((255 - color->r) - color->r));
	color->g += (0.2f * ((255 - color->g) - color->g));
	color->b += (0.2f * ((255 - color->b) - color->b));
}
