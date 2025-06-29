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
static inline float	apply_cone_equation(t_fvector3 o, t_fvector3 d,
						t_cone *cone, float amplifier);
static inline float	find_candidate(float sqrt_delta, float a, float b);
/* -------------------------------------------------------------------------- */

float	intersect_cone(t_ray *ray, t_object *object, float amplifier)
{
	t_cone		*cone;
	t_fvector2	amplified;
	t_fvector3	o;
	t_fvector3	d;
	t_fvector2	t;

	cone = (t_cone *)object;
	amplified = (t_fvector2){
		cone->base_diameter * amplifier,
		cone->height * amplifier
	};
	normalize_complex_object(&o, &d, *ray, (t_normal_object *)object);
	t.y = apply_cone_equation(o, d, cone, amplified.y);
	ray->extra = 0;
	if (cone->infinite || fabsf(d.y) < 1e-6f)
		return (t.y);
	t.x = intersect_cap(o, d, amplified.x * 0.5f, amplified.y);
	ray->extra = t.x > EPSILON && (t.y < 0 || t.x < t.y);
	if (ray->extra == 1)
		return (t.x);
	return (t.y);
}

static inline float	apply_cone_equation(t_fvector3 o, t_fvector3 d,
				t_cone *cone, float height)
{
	float	a;
	float	b;
	float	delta;
	float	t;
	float	y_hit;

	a = d.x * d.x + d.z * d.z - cone->k2 * d.y * d.y;
	b = 2.0f * (o.x * d.x + o.z * d.z - cone->k2 * o.y * d.y);
	delta = b * b - 4.0f * a * (o.x * o.x + o.z * o.z - cone->k2 * o.y * o.y);
	if (delta < 0.0f)
		return (-1.0f);
	t = find_candidate(sqrtf(delta), a, b);
	if (t < 0.0f)
		return (-1.0f);
	y_hit = o.y + t * d.y;
	if (!cone->infinite && (y_hit < 0.0f || y_hit > height))
		return (-1.0f);
	return (t);
}

static inline float	find_candidate(float sqrt_delta, float a, float b)
{
	float	t0;
	float	t1;

	if (fabsf(a) < 1e-6f)
	{
		if (fabsf(b) > 1e-6f)
			return (-1.0f);
		return (-1.0f);
	}
	t0 = (-b - sqrt_delta) / (2.0f * a);
	t1 = (-b + sqrt_delta) / (2.0f * a);
	if (t0 > 1e-6f && (t1 < 1e-6f || t0 < t1))
		return (t0);
	if (t1 > 1e-6f)
		return (t1);
	return (-1.0f);
}
