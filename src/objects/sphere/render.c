/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline float	intersection_sphere(t_ray ray, t_sphere *sphere);
static inline void	init_hit(t_ray *ray, t_hit_data *hit, t_sphere *sphere,
						float dist);
static inline t_rgb	checkerboard_pattern(t_hit_data hit);
/* -------------------------------------------------------------------------- */

void	render_sphere(t_minirt *mrt, t_ray *ray, t_object *object, int depth)
{
	t_ray		reflect_ray;
	t_sphere	*sphere;
	float		dist;
	t_hit_data	hit;
	int			inside;

	sphere = (t_sphere *)object;
	dist = intersection_sphere(*ray, sphere);
	if (dist <= 0 || dist > ray->dist)
		return ;
	init_hit(ray, &hit, sphere, dist);
	inside = ft_fvector3_length(ft_fvector3_diff(ray->origin,
				sphere->position)) < sphere->radius;
	if (inside)
		hit.normal = ft_fvector3_scale(hit.normal, -1);
	if (sphere->pattern == 1)
		sphere->color = checkerboard_pattern(hit);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit,
				sphere->radius * ((!inside) * -1 + (inside))),
			sphere->color);
	reflect_ray = *ray;
	specular_reflection(&reflect_ray, &hit, sphere->smoothness);
	ray->color = lerp(ray->color,
			ray_tracer(mrt, &reflect_ray, depth + 1), sphere->mat);
	ray->dist = dist;
}

static inline float	intersection_sphere(t_ray ray, t_sphere *sphere)
{
	t_fvector3	oc;
	float		b;
	float		delta;
	float		x1;
	float		x2;

	oc = ft_fvector3_diff(ray.origin, sphere->position);
	b = 2.0f * ft_fdot_product(oc, ray.direction);
	delta = b * b - 4.0f
		* (ft_fdot_product(oc, oc) - (sphere->radius * sphere->radius));
	if (delta < 0)
		return (-1.0f);
	x1 = (-b - sqrtf(delta)) / 2.0f;
	x2 = (-b + sqrtf(delta)) / 2.0f;
	if (x1 > 0.001f)
		return (x1);
	if (x2 > 0.001f)
		return (x2);
	return (-1.0f);
}

static inline void	init_hit(t_ray *ray, t_hit_data *hit, t_sphere *sphere,
	float dist)
{
	hit->impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, dist));
	hit->normal = ft_fnormalize(ft_fvector3_diff(hit->impact_point,
				sphere->position));
	hit->position = sphere->position;
}

static inline t_rgb	checkerboard_pattern(t_hit_data hit)
{
	if ((int)((floor((0.5f + atan2f(hit.normal.z, hit.normal.x)
					/ (2.0f * M_PI)) * 10.0f))
		+ (floor((0.5f - asinf(hit.normal.y) / M_PI) * 10.0f))) % 2 == 0)
		return ((t_rgb){255, 255, 255});
	else
		return ((t_rgb){0, 0, 0});
}
