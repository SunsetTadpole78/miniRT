/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:44:08 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/28 01:07:53 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline float	calculate_light_level(t_minirt *mrt, t_light *light,
						t_hit_data *hit, t_fvector3 direction);
static inline void	apply_diffuse_lights(t_minirt *mrt, t_light *light,
						t_hit_data *hit, t_frgb *color);
static inline int		is_light_blocked(t_minirt *mrt, t_ray *ray,
						t_light *light, t_object *object);
static inline float	get_distance(t_object *cur, t_fvector3 position, t_ray *ray,
						float (*intersect)(t_ray *, t_object *, float));
/* -------------------------------------------------------------------------- */

t_frgb	get_lights_modifier(t_minirt *mrt, t_hit_data *hit, int inside)
{
	t_frgb		color;
	t_light		*light;
	int			oid;

	color = mrt->ambiant->gamma_color;
	light = mrt->lights;
	oid = hit->object->oid;
	while (light)
	{
		if (!inside || is_light_inside(oid, light))
			apply_diffuse_lights(mrt, light, hit, &color);
		light = (t_light *)light->next;
	}
	if (color.r > 1.0f)
		color.r = 1.0f;
	if (color.g > 1.0f)
		color.g = 1.0f;
	if (color.b > 1.0f)
		color.b = 1.0f;
	return (color);
}

static inline void	apply_diffuse_lights(t_minirt *mrt, t_light *light,
		t_hit_data *hit, t_frgb *color)
{
	float		level;
	t_fvector3	pos;
	t_fvector3	impact;

	pos = light->position;
	impact = hit->impact_point;
	level = calculate_light_level(mrt, light, hit, (t_fvector3){
			pos.x - impact.x,
			pos.y - impact.y,
			pos.z - impact.z
		}) * light->level;
	color->r += light->gamma_color.r * level;
	color->g += light->gamma_color.g * level;
	color->b += light->gamma_color.b * level;
	hit->level += level;
}

static inline float	calculate_light_level(t_minirt *mrt, t_light *light,
		t_hit_data *hit, t_fvector3 direction)
{
	float		distance;
	float		dot;
	t_ray		shadow_ray;

	distance = sqrtf(direction.x * direction.x + direction.y * direction.y
			+ direction.z * direction.z);
	direction = ft_fnormalize(direction);
	dot = hit->normal.x * direction.x + hit->normal.y * direction.y
		+ hit->normal.z * direction.z;
	if (dot <= 0.0f)
		return (0.0f);
	shadow_ray.origin = (t_fvector3){
		hit->impact_point.x + hit->normal.x * 0.000001f,
		hit->impact_point.y + hit->normal.y * 0.000001f,
		hit->impact_point.z + hit->normal.z * 0.000001f
	};
	shadow_ray.direction = direction;
	shadow_ray.dist = distance;
	if (is_light_blocked(mrt, &shadow_ray, light, hit->object))
		return (0.0f);
	return (dot * (light->level / (light->level + LINEAR_ATTENUATION_COEF
				* distance + QUADRATIC_ATTENUATION_COEF
				* distance * distance)));
}

static inline int	is_light_blocked(t_minirt *mrt, t_ray *ray, t_light *light,
		t_object *object)
{
	float		dist;
	float		max_dist;
	t_fvector3	pos;
	float		(*intersect)(t_ray *, t_object *, float);
	t_object	*cur;

	pos = (t_fvector3){
		light->position.x - ray->origin.x,
		light->position.y - ray->origin.y,
		light->position.z - ray->origin.z};
	max_dist = sqrtf(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
	pos = mrt->camera->position;
	cur = mrt->objects;
	while (cur)
	{
		intersect = cur->methods->intersect;
		if (cur != object && intersect)
		{
			dist = get_distance(cur, pos, ray, intersect);
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
