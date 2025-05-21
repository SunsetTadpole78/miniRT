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

t_fvector3	reflect(t_fvector3 v, t_fvector3 n)
{
	return (ft_fvector3_diff(v, ft_fvector3_scale(n, 2.0f
				* ft_fdot_product(v, n))));
}

t_fvector3	lerp(t_fvector3 a, t_fvector3 b, float t)
{
	return (ft_fvector3_sum(ft_fvector3_scale(a, 1.0f - t),
			ft_fvector3_scale(b, t)));
}

t_fvector3	random_seed(void)
{
	float	theta;
	float	u;
	float	r;

	theta = 2.0f * M_PI * ((float)rand() / (float)RAND_MAX);
	u = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
	r = sqrtf(1.0f - u * u);
	return ((t_fvector3){r * cosf(theta), r * sinf(theta), u});
}
