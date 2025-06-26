/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/25 12:50:34 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

float	intersect_sphere(t_ray *ray, t_object *object, float amplifier)
{
	t_sphere	*sphere;
	t_fvector3	oc;
	float		b;
	float		delta;
	float		x;

	sphere = (t_sphere *)object;
	oc = ft_fvector3_diff(ray->origin, sphere->position);
	b = 2.0f * (oc.x * ray->direction.x + oc.y * ray->direction.y
			+ oc.z * ray->direction.z);
	delta = b * b - 4.0f * ((oc.x * oc.x + oc.y * oc.y + oc.z * oc.z)
			- (sphere->radius * sphere->radius * amplifier));
	if (delta < 0.0f)
		return (-1.0f);
	x = (-b - sqrtf(delta)) * 0.5f;
	if (x > EPSILON)
		return (x);
	x = (-b + sqrtf(delta)) * 0.5f;
	if (x > EPSILON)
		return (x);
	return (-1.0f);
}
