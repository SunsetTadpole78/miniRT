/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 00:20:45 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 18:44:43 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb	get_base_color(t_cone *co, t_fvector3 impact_point,
						t_pattern pattern);
/* -------------------------------------------------------------------------- */

void	apply_lights_cone(t_minirt *mrt, t_ray *ray, t_object *object, int depth)
{
	t_cone		*cone;
	int			inside;
	t_hit_data	hit;
	t_ray		reflect_ray;

	cone = (t_cone *)object;
	inside = init_cone(ray, &hit, cone);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			get_base_color(cone, hit.impact_point, cone->pattern));
	if (!inside && cone->pattern.mattifying != 0.0f)
	{
		reflect_ray = *ray;
		specular_reflection(&reflect_ray, &hit,
			cone->pattern.smoothness_factor);
		ray->color = ft_rgb_lerp(ray->color, ray_tracer(mrt, &reflect_ray,
					depth + 1), cone->pattern.mattifying);
	}
	if (cone->selected)
		apply_selection_effect(&ray->color);
}

static inline t_rgb	get_base_color(t_cone *cone, t_fvector3 impact_point,
		t_pattern pattern)
{
	t_fvector3	diff;
	t_fvector3	local;
	float		angle;

	if (pattern.id != 'c')
		return (pattern.main_color);
	diff = ft_fvector3_diff(impact_point, cone->position);
	local = (t_fvector3){
		ft_fdot_product(diff, cone->right),
		ft_fdot_product(diff, ft_fnormalize(cone->normal)),
		ft_fdot_product(diff, cone->up)
	};
	angle = atan2f(local.z, local.x);
	if (angle < 0.0f)
		angle += 2.0f * M_PI;
	if (((int)(floor(angle * 3.0f + EPSILON))
		+ (int)(floor(local.y * 0.3f + EPSILON))) % 2 == 0)
		return (pattern.secondary_color);
	return (pattern.main_color);
}
