/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/02 16:15:23 by lroussel         ###   ########.fr       */
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
	b = 2.0f * ft_fdot_product(oc, ray->direction);
	delta = b * b - 4.0f * (ft_fdot_product(oc, oc)
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

int	init_sphere(t_ray *ray, t_hit_data *hit, t_sphere *sphere)
{
	int	inside;

	hit->object = (t_object *)sphere;
	hit->impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, ray->dist));
	hit->normal = ft_fnormalize(ft_fvector3_diff(hit->impact_point,
				sphere->position));
	hit->position = sphere->position;
	hit->diff = (t_fvector3){0.0f, 0.0f, 0.0f};
	hit->proj = (t_fvector3){0.0f, 0.0f, 0.0f};
	hit->u = 0.0f;
	hit->v = 0.0f;
	hit->h = 0.0f;
	inside = ft_fvector3_length(ft_fvector3_diff(ray->origin,
				sphere->position)) < sphere->radius;
	if (inside)
		hit->normal = ft_fvector3_scale(hit->normal, -1);
	return (inside);
}
