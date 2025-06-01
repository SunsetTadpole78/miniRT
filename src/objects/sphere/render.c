/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/30 14:28:18 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void	init_hit(t_ray *ray, t_hit_data *hit, t_sphere *sphere,
						float dist);
static inline int		is_inside(t_sphere *sphere, t_fvector3 origin,
						t_fvector3 *normal);
static inline t_rgb	get_base_color(t_fvector3 normal, t_pattern pattern);
/* -------------------------------------------------------------------------- */

void	render_sphere(t_minirt *mrt, t_ray *ray, t_object *object, int depth)
{
	t_ray		reflect_ray;
	t_sphere	*sphere;
	t_hit_data	hit;
	float		dist;
	int			inside;

	dist = intersect_sphere(ray, object, 1.0f);
	if (dist <= 0 || dist > ray->dist)
		return ;
	sphere = (t_sphere *)object;
	init_hit(ray, &hit, sphere, dist);
	inside = is_inside(sphere, ray->origin, &hit.normal);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			get_base_color(hit.normal, sphere->pattern));
	if (!inside)
	{
		reflect_ray = *ray;
		specular_reflection(&reflect_ray, &hit, sphere->pattern.smoothness);
		ray->color = ft_rgb_lerp(ray->color, ray_tracer(mrt, &reflect_ray,
					depth + 1), sphere->pattern.mattifying);
	}
	if (sphere->selected)
		apply_selection_effect(&ray->color);
	ray->dist = dist;
}

static inline void	init_hit(t_ray *ray, t_hit_data *hit, t_sphere *sphere,
	float dist)
{
	hit->object = (t_object *)sphere;
	hit->impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, dist));
	hit->normal = ft_fnormalize(ft_fvector3_diff(hit->impact_point,
				sphere->position));
	hit->position = sphere->position;
}

static inline int	is_inside(t_sphere *sphere, t_fvector3 origin,
	t_fvector3 *normal)
{
	int	inside;

	inside = ft_fvector3_length(ft_fvector3_diff(origin,
				sphere->position)) < sphere->radius;
	if (inside)
		*normal = ft_fvector3_scale(*normal, -1);
	return (inside);
}

static inline t_rgb	get_base_color(t_fvector3 normal, t_pattern pattern)
{
	if (pattern.id != 'c')
		return (pattern.main_color);
	if ((int)((floor((0.5f + atan2f(normal.z, normal.x)
					/ (2.0f * M_PI)) * 10.0f))
		+ (floor((0.5f - asinf(normal.y) / M_PI) * 10.0f))) % 2 == 0)
		return (pattern.secondary_color);
	return (pattern.main_color);
}
