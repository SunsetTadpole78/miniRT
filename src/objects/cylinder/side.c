/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   side.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 02:56:19 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/31 21:58:01 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* -------------------------------- PROTOTYPE ------------------------------- */
static inline float	find_candidate(float sqrt_delta, float a, float b);
/* -------------------------------------------------------------------------- */

float	apply_side_equation(t_fvector3 o, t_fvector3 d,
	t_cylinder *cylinder, float amplifier)
{
	float	a;
	float	b;
	float	delta;
	float	t;
	float	y_hit;

	a = ft_fhorizontal_magnitude(d);
	b = 2 * (o.x * d.x + o.z * d.z);
	delta = b * b - 4 * a * (ft_fhorizontal_magnitude(o)
			- cylinder->radius * cylinder->radius * amplifier);
	if (delta <= 0.0f)
		return (-1.0f);
	t = find_candidate(sqrtf(delta), a, b);
	y_hit = o.y + t * d.y;
	if (!cylinder->infinite && (y_hit < -cylinder->half_height
			|| y_hit > cylinder->half_height))
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
