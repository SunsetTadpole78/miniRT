/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blender.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb			pixel_to_rgb(t_mlx *mlx, int x, int y);
static inline unsigned int	rgb_to_uint(t_rgb color);
/* -------------------------------------------------------------------------- */

void	blend_colors(t_minirt *mrt, t_ray *ray, t_vector2 pos)
{
	int		count;
	t_mlx	*mlx;
	t_rgb	prev_color;

	mlx = mrt->mlx;
	count = mlx->count;
	prev_color = pixel_to_rgb(mlx, pos.x, pos.y);
	*((unsigned int *)(mlx->data + (pos.y * mlx->ll + pos.x * mlx->cl)))
		= rgb_to_uint((t_rgb){
			(unsigned char)((prev_color.r * count + ray->color.r)
				/ (count + 1)),
			(unsigned char)((prev_color.g * count + ray->color.g)
				/ (count + 1)),
			(unsigned char)((prev_color.b * count + ray->color.b)
				/ (count + 1))});
}

static inline t_rgb	pixel_to_rgb(t_mlx *mlx, int x, int y)
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
