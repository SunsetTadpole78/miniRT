/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_fmatrix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
t_fmatrix	mat4_identity(void);
t_fmatrix	mat4_mul(t_fmatrix a, t_fmatrix b);
t_fvector3	mat4_mult_point(t_fmatrix m, t_fvector3 p);
t_fvector3	mat4_mult_dir(t_fmatrix m, t_fvector3 d);
t_fmatrix	mat4_look_at(t_fvector3 eye, t_fvector3 target, t_fvector3 up);
/* -------------------------------------------------------------------------- */

// matrice identité
t_fmatrix	mat4_identity(void)
{
	t_fmatrix	mat;
	int			i;
	int			j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			if (i == j)
				mat.m[i][j] = 1.0f;
			else
				mat.m[i][j] = 0.0f;
			j++;
		}
		i++;
	}
	return (mat);
}

// multiplication de matrices
t_fmatrix	mat4_mul(t_fmatrix a, t_fmatrix b)
{
	t_fmatrix	res;
	int			i;
	int			j;
	int			k;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			res.m[i][j] = 0.0f;
			k = 0;
			while (k < 4)
			{
				res.m[i][j] += a.m[i][k] * b.m[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
	return (res);
}

// multiplier une matrice par un vecteur
t_fvector3	mat4_mult_point(t_fmatrix m, t_fvector3 p)
{
	float		x;
	float		y;
	float		z;
	float		w;

	x = m.m[0][0] * p.x + m.m[0][1] * p.y + m.m[0][2] * p.z + m.m[0][3];
	y = m.m[1][0] * p.x + m.m[1][1] * p.y + m.m[1][2] * p.z + m.m[1][3];
	z = m.m[2][0] * p.x + m.m[2][1] * p.y + m.m[2][2] * p.z + m.m[2][3];
	w = m.m[3][0] * p.x + m.m[3][1] * p.y + m.m[3][2] * p.z + m.m[3][3];
	if (w != 0.0f)
	{
		x /= w;
		y /= w;
		z /= w;
	}
	return (ft_fvector3(x, y, z));
}

// transforme une direction
t_fvector3	mat4_mult_dir(t_fmatrix m, t_fvector3 d)
{
	float		x;
	float		y;
	float		z;

	x = m.m[0][0] * d.x + m.m[0][1] * d.y + m.m[0][2] * d.z;
	y = m.m[1][0] * d.x + m.m[1][1] * d.y + m.m[1][2] * d.z;
	z = m.m[2][0] * d.x + m.m[2][1] * d.y + m.m[2][2] * d.z;
	return (ft_fvector3(x, y, z));
}

// view matrix
t_fmatrix	mat4_look_at(t_fvector3 eye, t_fvector3 target, t_fvector3 up)
{
	t_fvector3		zaxis;
	t_fvector3		xaxis;
	t_fvector3		yaxis;
	t_fmatrix		view;

	zaxis = ft_fnormalize(ft_fvector3_diff(eye, target));
	xaxis = ft_fnormalize(ft_fcross_product(up, zaxis));
	yaxis = ft_fcross_product(zaxis, xaxis);
	view = mat4_identity();
	view.m[0][0] = xaxis.x;
	view.m[0][1] = xaxis.y;
	view.m[0][2] = xaxis.z;
	view.m[0][3] = -ft_fdot_product(xaxis, eye);
	view.m[1][0] = yaxis.x;
	view.m[1][1] = yaxis.y;
	view.m[1][2] = yaxis.z;
	view.m[1][3] = -ft_fdot_product(yaxis, eye);
	view.m[2][0] = zaxis.x;
	view.m[2][1] = zaxis.y;
	view.m[2][2] = zaxis.z;
	view.m[2][3] = -ft_fdot_product(zaxis, eye);
	return (view);
}
