/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brightness.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:44:08 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/20 15:00:04 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static float	calculate_light_level(t_light *light, t_fvector3 impact_point,
					t_fvector3 normal);

t_rgb	calculate_brightness(t_minirt *mrt, t_fvector3 impact_point,
			t_fvector3 normal, t_rgb color)
{
	t_light		*light;
	float		level;
	float		brightness;

	brightness = 0.0f;
	light = mrt->lights;
	if (!light)
	{
		color.r = 0;
		color.g = 0;
		color.b = 0;
		return (color);
	}
	while (light)
	{
		level = calculate_light_level(light, impact_point, normal);
			color.r = (color.r + ((level / 2.0f)
						* (light->color.r - color.r)));
			color.g = (color.g + ((level / 2.0f)
						* (light->color.g - color.g)));
			color.b = (color.b + ((level / 2.0f)
					* (light->color.b - color.b)));
			brightness += level;
		light = (t_light *)light->next;
	}
	if (brightness > 1.0f)
		brightness = 1.0f;
	color.r *= brightness;
	color.g *= brightness;
	color.b *= brightness;
	return (color);
}

static float	calculate_light_level(t_light *light, t_fvector3 impact_point,
	t_fvector3 normal)
{
	t_fvector3	direction;
	float		distance;

	direction = ft_fvector3_diff(light->position, impact_point);
	distance = ft_fvector3_length(direction);
	direction = ft_fnormalize(direction);
	return (fabsf(ft_fdot_product(normal, direction))
		* (1.0f / (1.0f + light->linear_at_coef * distance
				+ light->quadratic_at_coef * distance * distance)));
}
