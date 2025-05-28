/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:44:08 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/27 20:36:33 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline float	calculate_light_level(t_minirt *mrt, t_light *light,
						t_hit_data hit);
static inline void	apply_diffuse_lights(t_minirt *mrt, t_light *light,
						t_hit_data hit, t_frgb *color);
static inline int		is_light_blocked(t_minirt *mrt, t_ray *ray,
						t_light *light);
/* -------------------------------------------------------------------------- */

t_frgb	get_lights_modifier(t_minirt *mrt, t_hit_data hit, int inside,
			int (*check_method)(t_hit_data, t_fvector3))
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
		if (!inside || (check_method && check_method(hit, light->position)))
			apply_diffuse_lights(mrt, light, hit, &color);
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

static inline float	calculate_light_level(t_minirt *mrt, t_light *light,
		t_hit_data hit)
{
	t_fvector3	direction;
	float		distance;
	float		dot;
	t_ray		shadow_ray;

	direction = ft_fvector3_diff(light->position, hit.impact_point);
	distance = ft_fvector3_length(direction);
	direction = ft_fnormalize(direction);
	dot = ft_fdot_product(hit.normal, direction);
	if (dot <= 0.0f)
		return (0.0f);
	shadow_ray.origin = ft_fvector3_sum(hit.impact_point,
			ft_fvector3_scale(hit.normal, EPSILON));
	shadow_ray.direction = direction;
	shadow_ray.dist = distance;
	if (is_light_blocked(mrt, &shadow_ray, light))
		return (0.0f);
	return (dot * (1.0f / (1.0f + LINEAR_ATTENUATION_COEF * distance
				+ QUADRATIC_ATTENUATION_COEF * distance * distance)));
}

static inline void	apply_diffuse_lights(t_minirt *mrt, t_light *light,
		t_hit_data hit, t_frgb *color)
{
	float	level;

	level = calculate_light_level(mrt, light, hit) * light->level;
	color->r += powf(light->color.r / 255.0f, GAMMA) * level;
	color->g += powf(light->color.g / 255.0f, GAMMA) * level;
	color->b += powf(light->color.b / 255.0f, GAMMA) * level;
}

static inline int	is_light_blocked(t_minirt *mrt, t_ray *ray, t_light *light)
{
	t_object	*cur;
	float		dist;
	float		max_dist;

	cur = mrt->objects;
	max_dist = ft_fvector3_length(ft_fvector3_diff(light->position,
				ray->origin));
	while (cur)
	{
		if (cur->intersect)
		{
			dist = cur->intersect(*ray, cur);
			if (dist > 0.0f && dist < max_dist)
				return (1);
		}
		cur = cur->next;
	}
	return (0);
}
