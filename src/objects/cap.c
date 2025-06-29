/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 02:56:27 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/30 01:48:18 by lroussel         ###   ########.fr       */
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

t_fvector3	cap_bump_mapping(t_cylinder *cy, t_mlx_image bump, t_hit_data hit)
{
	t_rgb		c;
	float		hc;
	float		du;
	float		dv;

	c = mlx_pixel_to_rgb(bump,
			(int)((hit.u - floorf(hit.u)) * (float)bump.width) % bump.width,
			(int)((hit.v - floorf(hit.v)) * (float)bump.height) % bump.height);
	hc = (c.r + c.g + c.b) / 3.0f;
	c = mlx_pixel_to_rgb(bump,
			(int)((hit.u + 1.0f / (float)bump.width - floorf(hit.u + 1.0f
						/ (float)bump.width)) * (float)bump.width) % bump.width,
			(int)((hit.v - floorf(hit.v)) * (float)bump.height) % bump.height);
	du = ((c.r + c.g + c.b) / 3.0f - hc) * 0.5f;
	c = mlx_pixel_to_rgb(bump,
			(int)((hit.u - floorf(hit.u)) * (float)bump.width) % bump.width,
			(int)((hit.v + 1.0f / (float)bump.height - floorf(hit.v + 1.0f
						/ (float)bump.height)) * (float)bump.height)
			% bump.height);
	dv = ((c.r + c.g + c.b) / 3.0f - hc) * 0.5f;
	return (ft_fnormalize(ft_fvector3_sum(hit.normal, ft_fvector3_sum(
					ft_fvector3_scale(cy->right, du),
					ft_fvector3_scale(cy->up, dv)))));
}
