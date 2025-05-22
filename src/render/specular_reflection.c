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

void	specular_reflection(t_ray *ray, float smoothness)
{
	ray->origin = ray->hit;
	ray->direction = lerp(ft_fnormalize(
				ft_fvector3_sum(ray->normal, random_seed())),
			reflect(ray->direction, ray->normal), smoothness);
}
