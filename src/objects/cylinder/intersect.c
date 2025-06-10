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
	ray->extra = t.x > EPSILON && (t.z < 0.0f || t.x < t.z);
	if (ray->extra == 1)
		t.z = t.x;
	if (t.y < EPSILON || (t.z >= 0.0f && t.y >= t.z))
		return (t.z);
	ray->extra = 2;
	return (t.y);
}
