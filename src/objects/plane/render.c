/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/02 11:08:27 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void	init_plane(t_ray *ray, t_hit_data *hit, t_plane *plane,
						float dist);
static inline t_rgb	get_base_color(t_plane *plane, t_hit_data hit,
						t_pattern pattern);
/* -------------------------------------------------------------------------- */

void	render_plane(t_minirt *mrt, t_ray *ray, t_object *object, int depth)
{
	t_ray		reflect_ray;
	float		dist;
	t_plane		*plane;
	t_hit_data	hit;

	dist = intersect_plane(ray, object, 1.0f);
	if (dist <= 0 || dist > ray->dist)
		return ;
	plane = (t_plane *)object;
	init_plane(ray, &hit, plane, dist);
	if (ft_fdot_product(ray->direction, hit.normal) > 0)
		hit.normal = ft_fvector3_scale(hit.normal, -1);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, 0),
			get_base_color(plane, hit, plane->pattern));
	if (plane->pattern.mattifying != 0.0f)
	{
		reflect_ray = *ray;
		specular_reflection(&reflect_ray, &hit,
			plane->pattern.smoothness_factor);
		ray->color = ft_rgb_lerp(ray->color, ray_tracer(mrt, &reflect_ray,
					depth + 1), plane->pattern.mattifying);
	}
	if (plane->selected)
		apply_selection_effect(&ray->color);
	ray->dist = dist;
}

static inline void	init_plane(t_ray *ray, t_hit_data *hit, t_plane *plane,
	float dist)
{
	hit->object = (t_object *)plane;
	hit->impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, dist));
	hit->normal = plane->normal;
	hit->position = plane->position;
}

static inline t_rgb	get_base_color(t_plane *plane, t_hit_data hit,
	t_pattern pattern)
{
	t_fvector3	diff;

	if (pattern.id != 'c')
		return (pattern.main_color);
	diff = ft_fvector3_diff(hit.impact_point, hit.position);
	if ((int)((floor(ft_fdot_product(diff, plane->right) * 0.05f))
		+ (floor(ft_fdot_product(diff, plane->up) * 0.05f))) % 2 == 0)
		return (pattern.secondary_color);
	return (pattern.main_color);
}
