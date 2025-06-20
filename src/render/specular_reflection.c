/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specular_reflection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/20 15:47:16 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_fvector3		random_seed(t_fvector3 center, float phi,
									float cos_theta,
									float sin_theta);
static inline float			fast_rand(void);
/* -------------------------------------------------------------------------- */

void	specular_reflection(t_ray *ray, t_hit_data *hit,
		float smoothness_factor)
{
	float		cos_theta;
	t_fvector3	direction;
	t_fvector3	normal;

	direction = ray->direction;
	normal = hit->normal;
	cos_theta = 1.0f - smoothness_factor * (fast_rand() / 2147483647.0f);
	ray->origin = ft_fvector3_sum(hit->impact_point,
			ft_fvector3_scale(hit->normal, EPSILON));
	ray->direction = random_seed(
			ft_fvector3_diff(direction, ft_fvector3_scale(normal, 2.0f
					* ft_fdot_product(direction, normal))),
			2.0f * M_PI * (fast_rand() / 2147483647.0f),
			cos_theta,
			sqrtf(1.0f - cos_theta * cos_theta));
}

static inline t_fvector3	random_seed(t_fvector3 center, float phi,
	float cos_theta, float sin_theta)
{
	t_fvector3		w;
	t_fvector3		u;

	w = ft_fnormalize(center);
	if (fabsf(w.x) > 0.1f)
		u = ft_fnormalize(ft_fcross_product((t_fvector3){0, 1, 0}, w));
	else
		u = ft_fnormalize(ft_fcross_product((t_fvector3){1, 0, 0}, w));
	return (ft_fnormalize(ft_fvector3_sum(
				ft_fvector3_sum(ft_fvector3_scale(u, sin_theta * cosf(phi)),
					ft_fvector3_scale(ft_fcross_product(w, u),
						sin_theta * sinf(phi))),
				ft_fvector3_scale(w, cos_theta))));
}

static inline float	fast_rand(void)
{
	static _Thread_local unsigned int	seed = 123456789;

	seed = (1103515245 * seed + 12345) & 0x7fffffff;
	return ((float)seed);
}
