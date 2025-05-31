/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_pixel_to_rgb.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_rgb	texture_pixel_to_rgb(t_texture *tex, int x, int y)
{
	unsigned int	pixel;

	pixel = *((unsigned int *)(tex->data + (y * tex->ll + x * tex->cl)));
	return ((t_rgb){(pixel >> 16) & 0xFF, (pixel >> 8) & 0xFF, pixel & 0xFF});
}
