/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:44:08 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/22 13:11:32 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static inline float	calculate_light_level(t_light *light,
						t_fvector3 impact_point, t_fvector3 normal);
static inline void	apply_diffuse_lights(t_light *light,
						t_fvector3 impact_point, t_fvector3 normal,
						t_frgb *color);

t_frgb	get_lights_modifier(t_minirt *mrt, t_fvector3 impact_point,
		t_fvector3 normal, float radius, t_fvector3 position)
{
	t_frgb	light_color;
	t_light	*light;

	light_color = (t_frgb){
		powf(mrt->ambiant->color.r / 255.0f, GAMMA) * mrt->ambiant->level,
		powf(mrt->ambiant->color.g / 255.0f, GAMMA) * mrt->ambiant->level,
		powf(mrt->ambiant->color.b / 255.0f, GAMMA) * mrt->ambiant->level};
	light = mrt->lights;
	while (light)
	{
		if (radius <= 0.0f
			|| ft_fvector3_length(ft_fvector3_diff(light->position, position))
			<= radius)
			apply_diffuse_lights(light, impact_point, normal, &light_color);
		light = (t_light *)light->next;
	}
	light_color.r = fmin(light_color.r, 1.0f);
	light_color.g = fmin(light_color.g, 1.0f);
	light_color.b = fmin(light_color.b, 1.0f);
	return (light_color);
}

t_rgb	apply_lights_modifier(t_frgb modifier, t_rgb base)
{
	base.r = (unsigned char) fmin(powf((powf((float)base.r / 255.0f, GAMMA))
				* fmin(modifier.r, 1.0f), 1.0f / GAMMA) * 255.0f, 255.0f);
	base.g = (unsigned char) fmin(powf((powf((float)base.g / 255.0f, GAMMA))
				* fmin(modifier.g, 1.0f), 1.0f / GAMMA) * 255.0f, 255.0f);
	base.b = (unsigned char) fmin(powf((powf((float)base.b / 255.0f, GAMMA))
				* fmin(modifier.b, 1.0f), 1.0f / GAMMA) * 255.0f, 255.0f);
	return (base);
}

static inline float	calculate_light_level(t_light *light,
		t_fvector3 impact_point, t_fvector3 normal)
{
	t_fvector3	direction;
	float		distance;
	float		dot;

	direction = ft_fvector3_diff(light->position, impact_point);
	distance = ft_fvector3_length(direction);
	direction = ft_fnormalize(direction);
	dot = ft_fdot_product(normal, direction);
	if (dot <= 0.0f)
		return (0.0f);
	return (dot * (1.0f / (1.0f + light->linear_at_coef * distance
				+ light->quadratic_at_coef * distance * distance)));
}

static inline void	apply_diffuse_lights(t_light *light,
		t_fvector3 impact_point, t_fvector3 normal, t_frgb *color)
{
	float	level;

	level = calculate_light_level(light, impact_point, normal) * light->level;
	color->r += powf(light->color.r / 255.0f, GAMMA) * level;
	color->g += powf(light->color.g / 255.0f, GAMMA) * level;
	color->b += powf(light->color.b / 255.0f, GAMMA) * level;
}
