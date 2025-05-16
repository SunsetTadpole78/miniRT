/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_math_fvector3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
t_fvector3		normalize(t_fvector3 v);
t_fvector3		cross(t_fvector3 v1, t_fvector3 v2);
t_fvector3		add_vectors(t_fvector3 v1, t_fvector3 v2);
t_fvector3		sub_vectors(t_fvector3 v1, t_fvector3 v2);
float			dot(t_fvector3 v1, t_fvector3 v2);
/* -------------------------------------------------------------------------- */

// Normaliser un vector 3D
t_fvector3	normalize(t_fvector3 v)
{
	t_fvector3		res;
	float			len;

	len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (!len)
		exit(1);
	res.x = v.x / len;
	res.y = v.y / len;
	res.z = v.z / len;
	return (res);
}

// Produit vectoriel
// |a| * |b| * sin(teta) * n
t_fvector3	cross(t_fvector3 v1, t_fvector3 v2)
{
	t_fvector3		res;

	res.x = v1.y * v2.z - v1.z * v2.y;
	res.y = v1.z * v2.x - v1.x * v2.z;
	res.z = v1.x * v2.y - v1.y * v2.x;
	return (res);
}

// Produit scalaire de 2 vectors. = dot product
float	dot(t_fvector3 v1, t_fvector3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

// Addition de 2 vecteurs
t_fvector3	add_vectors(t_fvector3 v1, t_fvector3 v2)
{
	t_fvector3		res;

	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	res.z = v1.z + v2.z;
	return (res);
}

// Soustraction de 2 vecteurs
t_fvector3	sub_vectors(t_fvector3 v1, t_fvector3 v2)
{
	t_fvector3		res;

	res.x = v1.x - v2.x;
	res.y = v1.y - v2.y;
	res.z = v1.z - v2.z;
	return (res);
}
