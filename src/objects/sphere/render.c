/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/03 02:47:42 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb	get_base_color(t_fvector3 normal, t_pattern pattern);
/* -------------------------------------------------------------------------- */

void	apply_lights_sphere(t_minirt *mrt, t_ray *ray, t_object *object,
		int depth)
{
	t_sphere	*sphere;
	t_hit_data	hit;
	int			inside;
	t_ray		reflect_ray;

	sphere = (t_sphere *)object;
	inside = init_sphere(ray, &hit, sphere);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			get_base_color(hit.normal, sphere->pattern));
	if (!inside && sphere->pattern.mattifying != 0.0f)
	{
		reflect_ray = *ray;
		specular_reflection(&reflect_ray, &hit,
			sphere->pattern.smoothness_factor);
		ray->color = ft_rgb_lerp(ray->color, ray_tracer(mrt, &reflect_ray,
					depth + 1), sphere->pattern.mattifying);
	}
	if (sphere->selected)
		apply_selection_effect(&ray->color);
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
