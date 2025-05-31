/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 00:20:45 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/31 13:43:42 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline float	apply_cone_equation(t_fvector3 o, t_fvector3 d,
						t_cone *cone, float amplifier);
static inline float	find_candidate(float sqrt_delta, float a, float b);
/* -------------------------------------------------------------------------- */

void	render_cone(t_minirt *mrt, t_ray *ray, t_object *object, int depth)
{
	float		dist;
	t_cone		*cone;
	t_hit_data	hit;
	int			inside;

	(void)depth;
	dist = intersect_cone(ray, object, 1.0f);
	if (dist < 0.0f || dist > ray->dist)
		return ;
	cone = (t_cone *)object;
	init_cone_hit(ray, &hit, cone, dist);
	inside = is_inside_cone(object, ray->origin);
	if (inside)
		hit.normal = ft_fvector3_scale(hit.normal, -1);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			cone->pattern.main_color);
	ray->dist = dist;
}

float	intersect_cone(t_ray *ray, t_object *object, float amplifier)
{
	t_cone		*cone;
	t_fvector3	o;
	t_fvector3	d;
	t_fvector2	t;

	(void)amplifier;
	cone = (t_cone *)object;
	if (cone->height <= 0.0f || cone->base_diameter <= 0.0f)
		return (-1.0f);
	normalize_complex_object(&o, &d, *ray, (t_normal_object *)object);
	t.y = apply_cone_equation(o, d, cone, amplifier);
	if (fabsf(d.y) < 1e-6f)
		return (t.y);
	t.x = intersect_cap(o, d, cone->base_diameter / 2.0f, cone->height);
	ray->extra = t.x > EPSILON && (t.y < 0 || t.x < t.y);
	if (ray->extra == 1)
		return (t.x);
	return (t.y);
}

static inline float	apply_cone_equation(t_fvector3 o, t_fvector3 d,
				t_cone *cone, float amplifier)
{
	float	a;
	float	b;
	float	delta;
	float	t;
	float	y_hit;

	(void)amplifier;
	a = d.x * d.x + d.z * d.z - cone->k2 * d.y * d.y;
	b = 2.0f * (o.x * d.x + o.z * d.z - cone->k2 * o.y * d.y);
	delta = b * b - 4.0f * a * (o.x * o.x + o.z * o.z - cone->k2 * o.y * o.y);
	if (delta < 0.0f)
		return (-1.0f);
	t = find_candidate(sqrtf(delta), a, b);
	if (t < 0.0f)
		return (-1.0f);
	y_hit = o.y + t * d.y;
	if (y_hit < 0.0f || y_hit > cone->height)
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
