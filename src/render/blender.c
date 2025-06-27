/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blender.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/28 01:12:55 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
	base.r = 255;
	if (temp.r <= 1.0f)
		base.r = (unsigned char)(temp.r * 255.0f);
	base.g = 255;
	if (temp.g <= 1.0f)
		base.g = (unsigned char)(temp.g * 255.0f);
	base.b = 255;
	if (temp.b <= 1.0f)
		base.b = (unsigned char)(temp.b * 255.0f);
	return (base);
}

void	apply_selection_effect(t_rgb *color)
{
	color->r += (0.2f * ((255 - color->r) - color->r));
	color->g += (0.2f * ((255 - color->g) - color->g));
	color->b += (0.2f * ((255 - color->b) - color->b));
}
