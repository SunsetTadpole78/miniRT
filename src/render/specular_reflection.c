/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specular_reflection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_fvector3	perfect_reflection(t_fvector3 v, t_fvector3 n);
static inline t_fvector3	random_seed(t_fvector3 center, float phi,
								float cos_theta,
								float sin_theta);
/* -------------------------------------------------------------------------- */

void	specular_reflection(t_ray *ray, t_hit_data *hit, t_pattern pattern)
{
	float	cos_theta;

	cos_theta = 1.0f - (1.0f - cosf(RAD_ANGLE * (1.0f - smoothness)))
		* ((float)rand() / (float)RAND_MAX);
	ray->origin = ft_fvector3_sum(hit->impact_point,
			ft_fvector3_scale(hit->normal, EPSILON));
	ray->direction = random_seed(perfect_reflection(
				ray->direction, hit->normal),
			2.0f * M_PI * ((float)rand() / (float)RAND_MAX),
			cos_theta,
			sqrtf(1.0f - cos_theta * cos_theta));
}

static inline t_fvector3	perfect_reflection(t_fvector3 v, t_fvector3 n)
{
	return (ft_fvector3_diff(v, ft_fvector3_scale(n, 2.0f
				* ft_fdot_product(v, n))));
}

static inline t_fvector3	random_seed(t_fvector3 center, float phi,
	float cos_theta, float sin_theta)
{
	t_fvector3		w;
	t_fvector3		u;
	t_fvector3		v;
	t_fvector3		sample;

	w = ft_fnormalize(center);
	if (fabsf(w.x) > 0.1f)
		u = ft_fnormalize(ft_fcross_product((t_fvector3){0, 1, 0}, w));
	else
		u = ft_fnormalize(ft_fcross_product((t_fvector3){1, 0, 0}, w));
	v = ft_fcross_product(w, u);
	sample.x = sin_theta * cosf(phi);
	sample.y = sin_theta * sinf(phi);
	sample.z = cos_theta;
	return (ft_fnormalize(ft_fvector3_sum(
				ft_fvector3_sum(ft_fvector3_scale(u, sample.x),
					ft_fvector3_scale(v, sample.y)),
				ft_fvector3_scale(w, sample.z))));
}
