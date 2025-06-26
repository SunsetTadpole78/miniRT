/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/25 12:52:03 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

float	intersect_plane(t_ray *ray, t_object *object, float amplifier)
{
	t_plane		*plane;
	float		denominator;
	float		x;
	t_fvector3	diff;

	(void)amplifier;
	plane = (t_plane *)object;
	denominator = ray->direction.x * plane->normal.x + ray->direction.y
		* plane->normal.y + ray->direction.z * plane->normal.z;
	if (fabsf(denominator) < 0.0001f)
		return (-1.0f);
	diff = ft_fvector3_diff(plane->position, ray->origin);
	x = (diff.x * plane->normal.x + diff.y * plane->normal.y
			+ diff.z * plane->normal.z) / denominator;
	if (x >= 0.0f)
		return (x);
	return (-1.0f);
}
