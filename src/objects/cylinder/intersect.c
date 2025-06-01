/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_fvector3	get_cylinder_normal(int type,
								t_fvector3 impact_point,
								t_cylinder *cylinder);
/* -------------------------------------------------------------------------- */

float	intersect_cylinder(t_ray *ray, t_object *object, float amplifier)
{
	t_cylinder	*cylinder;
	t_fvector3	o;
	t_fvector3	d;
	t_fvector3	t;
	t_fvector2	amplified;

	cylinder = (t_cylinder *)object;
	normalize_complex_object(&o, &d, *ray, (t_normal_object *)object);
	t.z = apply_side_equation(o, d, cylinder, amplifier);
	if (fabsf(d.y) < 1e-6f)
		return (t.z);
	ray->extra = 0;
	if (cylinder->infinite)
		return (t.z);
	amplified = (t_fvector2){cylinder->radius * amplifier,
		cylinder->half_height * amplifier};
	t.x = intersect_cap(o, d, amplified.x, -amplified.y);
	t.y = intersect_cap(o, d, amplified.x, amplified.y);
	ray->extra = t.x > EPSILON && (t.z < 0 || t.x < t.z);
	if (ray->extra == 1)
		t.z = t.x;
	if (t.y < EPSILON || (t.z >= 0 && t.y >= t.z))
		return (t.z);
	ray->extra = 2;
	return (t.y);
}

int	init_cylinder(t_ray *ray, t_hit_data *hit, t_cylinder *cylinder, float dist)
{
	int	inside;

	hit->object = (t_object *)cylinder;
	hit->impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, dist));
	hit->normal = get_cylinder_normal(ray->extra, hit->impact_point, cylinder);
	hit->position = cylinder->position;
	inside = is_inside_cylinder((t_object *)cylinder, ray->origin);
	if (inside)
		hit->normal = ft_fvector3_scale(hit->normal, -1);
	return (inside);
}

static inline t_fvector3	get_cylinder_normal(int type,
	t_fvector3 impact_point, t_cylinder *cylinder)
{
	if (type == 1)
		return (ft_fvector3_scale(cylinder->normal, -1.0f));
	if (type == 2)
		return (cylinder->normal);
	return (ft_fnormalize(ft_fvector3_diff(impact_point,
				ft_fvector3_sum(cylinder->position,
					ft_fvector3_scale(cylinder->normal,
						ft_fdot_product(ft_fvector3_diff(
								impact_point,
								cylinder->position),
							cylinder->normal))))));
}
