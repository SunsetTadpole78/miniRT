/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_fmatrix_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
t_fmatrix	mat4_translation(t_fvector3 pos);
t_fmatrix	mat4_scaling(t_fvector3 scale);
t_ray		ray_to_space_object(t_fmatrix mat, t_ray ray);
/* -------------------------------------------------------------------------- */

t_fmatrix	mat4_translation(t_fvector3 pos)
{
	t_fmatrix		mat;

	mat = mat4_identity();
	mat.m[0][3] = pos.x;
	mat.m[1][3] = pos.y;
	mat.m[2][3] = pos.z;
	return (mat);
}

t_fmatrix	mat4_scaling(t_fvector3 scale)
{
	t_fmatrix		mat;

	mat = mat4_identity();
	mat.m[0][0] = scale.x;
	mat.m[1][1] = scale.y;
	mat.m[2][2] = scale.z;
	return (mat);
}

// Transforme un rayon monde en espace objet.
t_ray	ray_to_space_object(t_fmatrix mat, t_ray ray)
{
	t_ray		res;

	res.origin = mat4_mult_point(mat, ray.origin);
	res.direction = ft_fnormalize(mat4_mult_dir(mat, ray.direction));
	res.t = ray.t;
	return (res);
}
