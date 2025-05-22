/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline float			intersection_plane(t_ray ray, t_plane *plane);
static inline unsigned int	checkerboard_pattern(t_plane *plane,
								t_ray *ray, float dist);
/* -------------------------------------------------------------------------- */

void	render_plane(t_minirt *mrt, t_ray *ray, t_object *object)
{
	t_mlx			*mlx;
	t_plane			*plane;
	float			dist;

	mlx = mrt->mlx;
	plane = (t_plane *)object;
	dist = intersection_plane(*ray, plane);
	if (dist > 0 && dist <= ray->dist)
	{
		ray->hit = ft_fvector3_sum(ray->origin,
				ft_fvector3_scale(ray->direction, dist));
		ray->normal = ft_fnormalize(
				ft_fvector3_diff(ray->hit, plane->position));
		if (plane->pattern >= 1)
			mlx->pixel_color = checkerboard_pattern(plane, ray, dist);
		else
			mlx->pixel_color = plane->color.r << 16
				| plane->color.g << 8 | plane->color.b;
		specular_reflection(ray, 1.0f);
		ray->dist = dist;
	}
}

// t = ((P - O) * N) / D * N
// P = point du plan.		O = origin du rayon.
// D = direction du rayon.	N = normal du plan.
static inline float	intersection_plane(t_ray ray, t_plane *plane)
{
	float	denominator;
	float	x;

	denominator = ft_fdot_product(ray.direction, plane->normal);
	if (fabs(denominator) < 0.000001f)
		return (-1.0f);
	x = ft_fdot_product(ft_fvector3_diff(plane->position, ray.origin),
			plane->normal) / denominator;
	if (x >= 0.0f)
		return (x);
	return (-1.0f);
}

static inline unsigned int	checkerboard_pattern(t_plane *plane,
	t_ray *ray, float dist)
{
	t_fvector3	diff;

	diff = ft_fvector3_diff(ft_fvector3_sum(ray->origin,
				ft_fvector3_scale(ray->direction, dist)), plane->position);
	if ((int)((floor(ft_fdot_product(diff, plane->right) * 0.05f))
		+ (floor(ft_fdot_product(diff, plane->up) * 0.05f))) % 2 == 0)
		return (0xFFFFFF);
	else
		return (0x000000);
}
