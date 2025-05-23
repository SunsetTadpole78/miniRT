/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:44:08 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/22 17:44:25 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static inline float	calculate_light_level(t_light *light,
						t_fvector3 impact_point, t_fvector3 normal);
static inline void	apply_diffuse_lights(t_light *light,
						t_fvector3 impact_point, t_fvector3 normal,
						t_frgb *color);

t_frgb	get_lights_modifier(t_minirt *mrt, t_hit_data hit, float radius)
{
	t_frgb	color;
	t_light	*light;

	color = (t_frgb){
		(float)powf(mrt->ambiant->color.r / 255.0f, GAMMA)
		* mrt->ambiant->level,
		(float)powf(mrt->ambiant->color.g / 255.0f, GAMMA)
		* mrt->ambiant->level,
		(float)powf(mrt->ambiant->color.b / 255.0f, GAMMA)
		* mrt->ambiant->level};
	light = mrt->lights;
	while (light)
	{
		if (radius <= 0.0f || ft_fvector3_length(ft_fvector3_diff(
					light->position, hit.position)) <= radius)
			apply_diffuse_lights(light, hit.impact_point, hit.normal, &color);
		light = (t_light *)light->next;
	}
	color.r = fmin(color.r, 1.0f);
	color.g = fmin(color.g, 1.0f);
	color.b = fmin(color.b, 1.0f);
	return (color);
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
	return (dot * (1.0f / (1.0f + LINEAR_ATTENUATION_COEF * distance
				+ QUADRATIC_ATTENUATION_COEF * distance * distance)));
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
