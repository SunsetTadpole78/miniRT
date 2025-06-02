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

float	intersect_plane(t_ray *ray, t_object *object, float amplifier)
{
	t_plane	*plane;
	float	denominator;
	float	x;

	(void)amplifier;
	plane = (t_plane *)object;
	denominator = ft_fdot_product(ray->direction, plane->normal);
	if (fabsf(denominator) < 0.0001f)
		return (-1.0f);
	x = ft_fdot_product(ft_fvector3_diff(plane->position, ray->origin),
			plane->normal) / denominator;
	if (x >= 0.0f)
		return (x);
	return (-1.0f);
}
