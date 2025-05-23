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
static inline t_fvector3	reflect(t_fvector3 v, t_fvector3 n);
static inline t_fvector3	lerp_vector(t_fvector3 a, t_fvector3 b, float t);
static inline t_fvector3	random_seed(void);
/* -------------------------------------------------------------------------- */

void	specular_reflection(t_ray *ray, t_hit_data *hit, float smoothness)
{
	ray->origin = ft_fvector3_sum(hit->impact_point,
			ft_fvector3_scale(hit->normal, EPSILON));
	ray->direction = lerp_vector(ft_fnormalize(
				ft_fvector3_sum(hit->normal, random_seed())),
			reflect(ray->direction, hit->normal), smoothness);
}

static inline t_fvector3	reflect(t_fvector3 v, t_fvector3 n)
{
	return (ft_fvector3_diff(v, ft_fvector3_scale(n, 2.0f
				* ft_fdot_product(v, n))));
}

static inline t_fvector3	lerp_vector(t_fvector3 a, t_fvector3 b, float t)
{
	return (ft_fvector3_sum(ft_fvector3_scale(a, 1.0f - t),
			ft_fvector3_scale(b, t)));
}

static inline t_fvector3	random_seed(void)
{
	float	theta;
	float	u;
	float	r;

	theta = 2.0f * M_PI * ((float)rand() / (float)RAND_MAX);
	u = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
	r = sqrtf(1.0f - u * u);
	return ((t_fvector3){r * cosf(theta), r * sinf(theta), u});
}
