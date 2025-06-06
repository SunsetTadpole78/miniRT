/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/03 00:28:44 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb		get_base_color(t_cylinder *cy, t_fvector3 impact_point,
							t_pattern pattern);
/* -------------------------------------------------------------------------- */

void	apply_lights_cylinder(t_minirt *mrt, t_ray *ray, t_object *object,
		int depth)
{
	t_cylinder	*cylinder;
	t_hit_data	hit;
	int			inside;
	t_ray		reflect_ray;

	cylinder = (t_cylinder *)object;
	inside = init_cylinder(ray, &hit, cylinder);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			get_base_color(cylinder, hit.impact_point, cylinder->pattern));
	if (!inside && cylinder->pattern.mattifying != 0.0f)
	{
		reflect_ray = *ray;
		specular_reflection(&reflect_ray, &hit,
			cylinder->pattern.smoothness_factor);
		ray->color = ft_rgb_lerp(ray->color, ray_tracer(mrt, &reflect_ray,
					depth + 1), cylinder->pattern.mattifying);
	}
	if (cylinder->selected)
		apply_selection_effect(&ray->color);
}

static inline t_rgb	get_base_color(t_cylinder *cy, t_fvector3 impact_point,
	t_pattern pattern)
{
	t_fvector3	diff;
	t_fvector3	proj;
	float		h;
	float		angle;

	if (pattern.id != 'c')
		return (pattern.main_color);
	diff = ft_fvector3_diff(impact_point, cy->position);
	h = ft_fdot_product(diff, cy->normal);
	proj = ft_fvector3_diff(diff, ft_fvector3_scale(cy->normal, h));
	angle = atan2f(ft_fdot_product(proj, cy->up),
			ft_fdot_product(proj, cy->right));
	if (angle < 0)
		angle += 2.0f * M_PI;
	if ((int)(floor(angle * 3.0f + EPSILON)
		+ floor((h + cy->half_height) * 0.3f + EPSILON)) % 2 == 0)
		return (pattern.secondary_color);
	return (pattern.main_color);
}
