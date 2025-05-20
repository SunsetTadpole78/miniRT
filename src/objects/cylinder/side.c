/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   side.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 02:56:19 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/20 02:59:30 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* -------------------------------- PROTOTYPE ------------------------------- */
static float	find_candidate(float sqrt_delta, float a, float b);
/* -------------------------------------------------------------------------- */

void	normalize_side(t_fvector3 *local_origin, t_fvector3 *local_dir,
	t_ray ray, t_cylinder *cylinder)
{
	t_fvector3	oc;
	t_fvector3	up;
	t_fvector3	right;
	t_fvector3	forward;

	oc = ft_fvector3_diff(ray.origin, cylinder->position);
	up = ft_fnormalize(cylinder->normal);
	if (fabsf(up.y) < 0.999f)
		right = ft_fnormalize(
				ft_fcross_product(ft_fvector3(0.0f, 1.0f, 0.0f), up));
	else
		right = ft_fnormalize(
				ft_fcross_product(ft_fvector3(1.0f, 0.0f, 0.0f), up));
	forward = ft_fcross_product(up, right);
	*local_origin = (t_fvector3){
		ft_fdot_product(oc, right),
		ft_fdot_product(oc, up),
		ft_fdot_product(oc, forward)
	};
	*local_dir = ft_fvector3(
			ft_fdot_product(ray.direction, right),
			ft_fdot_product(ray.direction, up),
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

	a = local_dir.x * local_dir.x + local_dir.z * local_dir.z;
	b = 2 * (local_origin.x * local_dir.x + local_origin.z * local_dir.z);
	delta = b * b - 4 * a * (local_origin.x * local_origin.x + local_origin.z
			* local_origin.z - cylinder->radius * cylinder->radius);
	t = -1.0f;
	if (delta > 0.0f)
	{
		t = find_candidate(sqrtf(delta), a, b);
		y_hit = local_origin.y + t * local_dir.y;
		if (y_hit < -cylinder->half_height || y_hit > cylinder->half_height)
			t = -1.0f;
	}
	return (t);
}

static float	find_candidate(float sqrt_delta, float a, float b)
{
	float	x1;
	float	x2;

	x1 = (-b - sqrt_delta) / (2.0f * a);
	if (x1 > 0.001f)
		return (x1);
	x2 = (-b + sqrt_delta) / (2.0f * a);
	if (x2 > 0.001f)
		return (x2);
	return (-1.0f);
}
