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
static inline float	intersection_plane(t_ray ray, t_plane *plane);
static inline void	init_hit(t_ray *ray, t_hit_data *hit, t_plane *plane,
						float dist);
static inline t_rgb	checkerboard_pattern(t_plane *plane, t_hit_data hit);
/* -------------------------------------------------------------------------- */

void	render_plane(t_minirt *mrt, t_ray *ray, t_object *object, int depth)
{
	t_ray		reflect_ray;
	t_plane		*plane;
	float		dist;
	t_hit_data	hit;

	plane = (t_plane *)object;
	dist = intersection_plane(*ray, plane);
	if (dist <= 0 || dist > ray->dist)
		return ;
	init_hit(ray, &hit, plane, dist);
	if (ft_fdot_product(ray->direction, hit.normal) > 0)
		hit.normal = ft_fvector3_scale(hit.normal, -1);
	if (plane->pattern == 1)
		plane->color = checkerboard_pattern(plane, hit);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, 0),
			plane->color);
	reflect_ray = *ray;
	specular_reflection(&reflect_ray, &hit, plane->smoothness);
	ray->color = ft_rgb_lerp(ray->color,
			ray_tracer(mrt, &reflect_ray, depth + 1), plane->mat);
	ray->dist = dist;
}

static inline float	intersection_plane(t_ray ray, t_plane *plane)
{
	float	denominator;
	float	x;

	denominator = ft_fdot_product(ray.direction, plane->normal);
	if (fabs(denominator) < 0.000001f)
		return (-1.0f);
	x = ft_fdot_product(ft_fvector3_diff(plane->position, ray.origin),
			plane->normal) / denominator;
	if (x >= 0.0f)
		return (x);
	return (-1.0f);
}

static inline void	init_hit(t_ray *ray, t_hit_data *hit, t_plane *plane,
	float dist)
{
	hit->impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, dist));
	hit->normal = plane->normal;
	hit->position = plane->position;
}

static inline t_rgb	checkerboard_pattern(t_plane *plane, t_hit_data hit)
{
	t_fvector3	diff;

	diff = ft_fvector3_diff(hit.impact_point, hit.position);
	if ((int)((floor(ft_fdot_product(diff, plane->right) * 0.05f))
		+ (floor(ft_fdot_product(diff, plane->up) * 0.05f))) % 2 == 0)
		return ((t_rgb){255, 255, 255});
	else
		return ((t_rgb){0, 0, 0});
}
