/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 21:31:27 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/31 12:54:19 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	normalize_complex_object(t_fvector3 *o, t_fvector3 *d, t_ray ray,
		t_normal_object *object)
{
	t_fvector3	oc;
	t_fvector3	right;
	t_fvector3	forward;

	oc = ft_fvector3_diff(ray.origin, object->position);
	if (fabsf(object->normal.y) < 0.999f)
		right = ft_fnormalize(ft_fcross_product((t_fvector3){0.0f, 1.0f, 0.0f},
					object->normal));
	else
		right = ft_fnormalize(ft_fcross_product((t_fvector3){1.0f, 0.0f, 0.0f},
					object->normal));
	forward = ft_fcross_product(object->normal, right);
	*o = (t_fvector3){
		ft_fdot_product(oc, right),
		ft_fdot_product(oc, object->normal),
		ft_fdot_product(oc, forward)
	};
	*d = (t_fvector3){
		ft_fdot_product(ray.direction, right),
		ft_fdot_product(ray.direction, object->normal),
		ft_fdot_product(ray.direction, forward)
	};
}
