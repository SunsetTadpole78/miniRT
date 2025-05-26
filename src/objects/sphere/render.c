/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/26 13:08:14 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void	init_hit(t_ray *ray, t_hit_data *hit, t_sphere *sphere,
						float dist);
/* -------------------------------------------------------------------------- */

void	render_sphere(t_minirt *mrt, t_ray *ray, t_object *object, int depth)
{
	t_ray		reflect_ray;
	float		dist;
	t_sphere	*sphere;
	t_hit_data	hit;
	int			inside;

	dist = intersect_sphere(*ray, object);
	if (dist <= 0 || dist > ray->dist)
		return ;
	sphere = (t_sphere *)object;
	init_hit(ray, &hit, sphere, dist);
	inside = ft_fvector3_length(ft_fvector3_diff(ray->origin,
				sphere->position)) < sphere->radius;
	if (inside)
		hit.normal = ft_fvector3_scale(hit.normal, -1);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit,
				sphere->radius * ((!inside) * -1 + (inside))),
			sphere->color);
	reflect_ray = *ray;
	specular_reflection(&reflect_ray, &hit, sphere->smoothness);
	ray->color = ft_rgb_lerp(ray->color,
			ray_tracer(mrt, &reflect_ray, depth + 1), sphere->mat);
	ray->dist = dist;
}

float	intersect_sphere(t_ray ray, t_object *object)
{
	t_sphere	*sphere;
	t_fvector3	oc;
	float		b;
	float		delta;
	float		x;

	sphere = (t_sphere *)object;
	oc = ft_fvector3_diff(ray.origin, sphere->position);
	b = 2.0f * ft_fdot_product(oc, ray.direction);
	delta = b * b - 4.0f
		* (ft_fdot_product(oc, oc) - (sphere->radius * sphere->radius));
	if (delta < 0.0f)
		return (-1.0f);
	x = (-b - sqrtf(delta)) / 2.0f;
	if (x > EPSILON)
		return (x);
	x = (-b + sqrtf(delta)) / 2.0f;
	if (x > EPSILON)
		return (x);
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
	hit->ray = ray;
}
