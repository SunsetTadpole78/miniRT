/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:44:08 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/29 14:55:08 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline float	calculate_light_level(t_minirt *mrt, t_light *light,
						t_hit_data hit);
static inline void	apply_diffuse_lights(t_minirt *mrt, t_light *light,
						t_hit_data hit, t_frgb *color);
static inline int		is_light_blocked(t_minirt *mrt, t_ray *ray,
						t_light *light, t_object *object);
static inline float	get_distance(t_object *cur, t_fvector3 position, t_ray *ray,
						float (*intersect)(t_ray *, t_object *, float));
/* -------------------------------------------------------------------------- */

t_frgb	get_lights_modifier(t_minirt *mrt, t_hit_data hit, int inside)
{
	t_frgb	color;
	t_light	*light;
	int		(*is_inside)(t_object *, t_fvector3);

	color = (t_frgb){
		(float)powf(mrt->ambiant->color.r / 255.0f, GAMMA)
		* mrt->ambiant->level,
		(float)powf(mrt->ambiant->color.g / 255.0f, GAMMA)
		* mrt->ambiant->level,
		(float)powf(mrt->ambiant->color.b / 255.0f, GAMMA)
		* mrt->ambiant->level};
	light = mrt->lights;
	is_inside = hit.object->methods->is_inside;
	while (light)
	{
		if (!inside || (is_inside && is_inside(hit.object, light->position)))
			apply_diffuse_lights(mrt, light, hit, &color);
		light = (t_light *)light->next;
	}
	color.r = fmin(color.r, 1.0f);
	color.g = fmin(color.g, 1.0f);
	color.b = fmin(color.b, 1.0f);
	return (color);
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
			ft_fvector3_scale(hit.normal, 0.000001f));
	shadow_ray.direction = direction;
	shadow_ray.dist = distance;
	if (is_light_blocked(mrt, &shadow_ray, light, hit.object))
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

static inline int	is_light_blocked(t_minirt *mrt, t_ray *ray, t_light *light,
		t_object *object)
{
	t_object	*cur;
	float		dist;
	float		max_dist;
	t_fvector3	position;
	float		(*intersect)(t_ray *, t_object *, float);

	cur = mrt->objects;
	max_dist = ft_fvector3_length(ft_fvector3_diff(light->position,
				ray->origin));
	position = mrt->camera->position;
	while (cur)
	{
		intersect = cur->methods->intersect;
		if (cur != object && intersect)
		{
			dist = get_distance(cur, position, ray, intersect);
			if (dist > 0.0f && dist <= max_dist)
				return (1);
		}
		cur = cur->next;
	}
	return (0);
}

static inline float	get_distance(t_object *cur, t_fvector3 position, t_ray *ray,
		float (*intersect)(t_ray *, t_object *, float))
{
	int	(*is_inside)(t_object *, t_fvector3);

	is_inside = cur->methods->is_inside;
	if (is_inside && !is_inside(cur, position))
		return (intersect(ray, cur, 0.999f));
	return (intersect(ray, cur, 1.0f));
}
