/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 02:56:27 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/30 21:30:12 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

float	intersect_cap(t_fvector3 o, t_fvector3 d, float radius,
		float half_height)
{
	float	t;
	float	x;
	float	z;

	if (fabsf(d.y) < EPSILON)
		return (-1.0f);
	t = (half_height - o.y) / d.y;
	if (t < EPSILON)
		return (-1.0f);
	x = o.x + t * d.x;
	z = o.z + t * d.z;
	if (x * x + z * z <= radius * radius)
		return (t);
	return (-1.0f);
}
