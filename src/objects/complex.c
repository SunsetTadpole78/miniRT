/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 21:31:27 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/25 13:04:11 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	normalize_complex_object(t_fvector3 *o, t_fvector3 *d, t_ray ray,
		t_normal_object *object)
{
	t_fvector3	oc;
	t_fvector3	right;
	t_fvector3	forward;
	t_fvector3	normal;
	t_fvector3	dir;

	oc = ft_fvector3_diff(ray.origin, object->position);
	normal = object->normal;
	if (fabsf(normal.y) < 0.999f)
		right = ft_fnormalize(ft_fcross_product((t_fvector3){0.0f, 1.0f, 0.0f},
					normal));
	else
		right = ft_fnormalize(ft_fcross_product((t_fvector3){1.0f, 0.0f, 0.0f},
					normal));
	forward = ft_fcross_product(normal, right);
	*o = (t_fvector3){
		oc.x * right.x + oc.y * right.y + oc.z * right.z,
		oc.x * normal.x + oc.y * normal.y + oc.z * normal.z,
		oc.x * forward.x + oc.y * forward.y + oc.z * forward.z};
	dir = ray.direction;
	*d = (t_fvector3){
		dir.x * right.x + dir.y * right.y + dir.z * right.z,
		dir.x * normal.x + dir.y * normal.y + dir.z * normal.z,
		dir.x * forward.x + dir.y * forward.y + dir.z * forward.z};
}
