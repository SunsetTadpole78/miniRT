/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specular_reflection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/26 20:49:48 by lroussel         ###   ########.fr       */
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
	t_fvector3	dir;
	t_fvector3	normal;

	dir = ray->direction;
	normal = hit->normal;
	cos_theta = 1.0f - smoothness_factor * (fast_rand() / 2147483647.0f);
	ray->origin = ft_fvector3_sum(hit->impact_point,
			ft_fvector3_scale(hit->normal, EPSILON));
	ray->direction = random_seed(
			ft_fvector3_diff(dir, ft_fvector3_scale(normal, 2.0f * (dir.x
						* normal.x + dir.y * normal.y + dir.z * normal.z))),
			2.0f * M_PI * (fast_rand() / 2147483647.0f),
			cos_theta,
			sqrtf(1.0f - cos_theta * cos_theta));
}

static inline t_fvector3	random_seed(t_fvector3 center, float phi,
	float cos_theta, float sin_theta)
{
	t_fvector3	w;
	t_fvector3	u;
	float		scale1;
	float		scale2;

	w = ft_fnormalize(center);
	if (fabsf(w.x) > 0.1f)
		u = ft_fnormalize(ft_fcross_product((t_fvector3){0, 1, 0}, w));
	else
		u = ft_fnormalize(ft_fcross_product((t_fvector3){1, 0, 0}, w));
	scale1 = sin_theta * cosf(phi);
	scale2 = sin_theta * sinf(phi);
	return (ft_fnormalize((t_fvector3){
			(u.x * scale1) + ((w.y * u.z - w.z * u.y) * scale2)
			+ (w.x * cos_theta),
			(u.y * scale1) + ((w.z * u.x - w.x * u.z) * scale2)
			+ (w.y * cos_theta),
			(u.z * scale1) + ((w.x * u.y - w.y * u.x) * scale2)
			+ (w.z * cos_theta)
		}));
}

static inline float	fast_rand(void)
{
	static _Thread_local unsigned int	seed = 123456789;

	seed = (1103515245 * seed + 12345) & 0x7fffffff;
	return ((float)seed);
}
