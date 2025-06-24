/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_pixel_to_rgb.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_rgb	mlx_pixel_to_rgb(t_mlx_image image, int x, int y)
{
	unsigned int	pixel;

	pixel = *((unsigned int *)(image.data + (y * image.ll + x * image.cl)));
	return ((t_rgb){(pixel >> 16) & 0xFF, (pixel >> 8) & 0xFF, pixel & 0xFF});
}
