/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
float		intersection_plane(t_ray ray, t_plane *plane);
float		intersection_sphere(t_ray ray, t_sphere *sphere);
/* -------------------------------------------------------------------------- */

// t = ((P - O) * N) / D * N
// P = point du plan.		O = origin du rayon.
// D = direction du rayon.	N = normal du plan.
float	intersection_plane(t_ray ray, t_plane *plane)
{
	t_fvector3		vec;
	float			denominator;
	float			t;

	denominator = dot(ray.direction, plane->normal);
	if (fabs(denominator) < 0.000001)
		return (-1.0f);
	vec = sub_vectors(plane->position, ray.origin);
	t = dot(vec, plane->normal) / denominator;
	if (t >= 0)
		return (t);
	return (-1.0f);
}

// discriminant = b^2 - 4c
// t1 = (-b - sqrt(discriminant)) / 2.0f;
// t2 = (-b + sqrt(discriminant)) / 2.0f;
float	intersection_sphere(t_ray ray, t_sphere *sphere)
{
	t_fvector3		oc;
	float			b;
	float			discriminant;
	float			t1;
	float			t2;

	oc = sub_vectors(ray.origin, sphere->position);
	b = 2.0f * dot(oc, ray.direction);
	discriminant = b * b - 4.0f
		* (dot(oc, oc) - (sphere->radius * sphere->radius));
	if (discriminant < 0)
		return (-1.0f);
	t1 = (-b - sqrtf(discriminant)) / 2.0f;
	t2 = (-b + sqrtf(discriminant)) / 2.0f;
	if (t1 > 0.001f)
		return (t1);
	if (t2 > 0.001f)
		return (t2);
	return (-1.0f);
}
