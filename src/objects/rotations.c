/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotations.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/25 00:11:47 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_fvector3	rotate_object(t_fvector3 v, t_fvector3 axis, float theta)
{
	t_fvector3	cross;
	float		cos_t;
	float		sin_t;
	float		dot;

	cos_t = cosf(theta);
	sin_t = sinf(theta);
	dot = axis.x * v.x + axis.y * v.y + axis.z * v.z;
	cross = ft_fcross_product(axis, v);
	return ((t_fvector3){
		v.x * cos_t + cross.x * sin_t + axis.x * dot * (1 - cos_t),
		v.y * cos_t + cross.y * sin_t + axis.y * dot * (1 - cos_t),
		v.z * cos_t + cross.z * sin_t + axis.z * dot * (1 - cos_t)});
}
