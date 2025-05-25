/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   side.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 02:56:19 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/25 22:13:09 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* -------------------------------- PROTOTYPE ------------------------------- */
static inline float	find_candidate(float sqrt_delta, float a, float b);
static inline float	horizontal_magnitude(t_fvector3 v);
/* -------------------------------------------------------------------------- */

void	normalize_side(t_fvector3 *local_origin, t_fvector3 *local_dir,
	t_ray ray, t_cylinder *cylinder)
{
	t_fvector3	oc;
	t_fvector3	right;
	t_fvector3	forward;

	oc = ft_fvector3_diff(ray.origin, cylinder->position);
	if (fabsf(cylinder->normal.y) < 0.999f)
		right = ft_fnormalize(ft_fcross_product((t_fvector3){0.0f, 1.0f, 0.0f},
					cylinder->normal));
	else
		right = ft_fnormalize(ft_fcross_product((t_fvector3){1.0f, 0.0f, 0.0f},
					cylinder->normal));
	forward = ft_fcross_product(cylinder->normal, right);
	*local_origin = (t_fvector3){
		ft_fdot_product(oc, right),
		ft_fdot_product(oc, cylinder->normal),
		ft_fdot_product(oc, forward)
	};
	*local_dir = ft_fvector3(
			ft_fdot_product(ray.direction, right),
			ft_fdot_product(ray.direction, cylinder->normal),
			ft_fdot_product(ray.direction, forward)
			);
}

float	apply_side_equation(t_fvector3 local_origin, t_fvector3 local_dir,
	t_cylinder *cylinder)
{
	float	a;
	float	b;
	float	delta;
	float	t;
	float	y_hit;

	a = horizontal_magnitude(local_dir);
	b = 2 * (local_origin.x * local_dir.x + local_origin.z * local_dir.z);
	delta = b * b - 4 * a * (horizontal_magnitude(local_origin)
			- cylinder->radius * cylinder->radius);
	if (delta <= 0.0f)
		return (-1.0f);
	t = find_candidate(sqrtf(delta), a, b);
	y_hit = local_origin.y + t * local_dir.y;
	if (y_hit < -cylinder->half_height || y_hit > cylinder->half_height)
		return (-1.0f);
	return (t);
}

static inline float	find_candidate(float sqrt_delta, float a, float b)
{
	float	x1;
	float	x2;
	float	denominator;

	denominator = 2.0f * a;
	x1 = (-b - sqrt_delta) / denominator;
	if (x1 > EPSILON)
		return (x1);
	x2 = (-b + sqrt_delta) / denominator;
	if (x2 > EPSILON)
		return (x2);
	return (-1.0f);
}

static inline float	horizontal_magnitude(t_fvector3 v)
{
	return (v.x * v.x + v.z * v.z);
}
