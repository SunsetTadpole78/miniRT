/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sphere.c                                    :+:      :+:    :+:   */
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
static inline float			intersection_sphere(t_ray ray, t_sphere *sphere);
static inline unsigned int	checkerboard_pattern_sphere(t_sphere *sphere,
								t_ray *ray, float dist);
static inline float			get_intensity(t_ray *ray, t_ambiant *ambiant,
								t_sphere *sphere, float dist);
/* -------------------------------------------------------------------------- */

void	render_sphere(t_minirt *mrt, t_ray *ray,
		t_vector2 pixel, t_object *object)
{
	t_mlx			*mlx;
	t_sphere		*sphere;
	float			dist;
	float			intensity;
	unsigned int	color;

	mlx = mrt->mlx;
	sphere = (t_sphere *)object;
	dist = intersection_sphere(*ray, sphere);
	if (dist > 0 && dist <= ray->dist)
	{
		intensity = get_intensity(ray, mrt->ambiant, sphere, dist);
		if (sphere->pattern == 1)
			color = checkerboard_pattern_sphere(sphere, ray, dist);
		else
			color = ((int)(sphere->color.r * intensity) << 16
					| (int)(sphere->color.g * intensity) << 8
					| (int)(sphere->color.b * intensity));
		*((unsigned int *)(mlx->data + (int)(pixel.y * mlx->ll
						+ pixel.x * mlx->cl))) = color;
		ray->dist = dist;
	}
}

// discriminant = b^2 - 4ac
// t1 = (-b - sqrt(discriminant)) / 2.0f;
// t2 = (-b + sqrt(discriminant)) / 2.0f;
static inline float	intersection_sphere(t_ray ray, t_sphere *sphere)
{
	t_fvector3	oc;
	float		b;
	float		delta;
	float		x1;
	float		x2;

	oc = ft_fvector3_diff(ray.origin, sphere->position);
	b = 2.0f * ft_fdot_product(oc, ray.direction);
	delta = b * b - 4.0f
		* (ft_fdot_product(oc, oc) - (sphere->radius * sphere->radius));
	if (delta < 0)
		return (-1.0f);
	x1 = (-b - sqrtf(delta)) / 2.0f;
	x2 = (-b + sqrtf(delta)) / 2.0f;
	if (x1 > 0.001f)
		return (x1);
	if (x2 > 0.001f)
		return (x2);
	return (-1.0f);
}

static inline unsigned int	checkerboard_pattern_sphere(t_sphere *sphere,
	t_ray *ray, float dist)
{
	t_fvector3	diff;

	diff = ft_fnormalize(ft_fvector3_diff(ft_fvector3_sum(ray->origin,
					ft_fvector3_scale(ray->direction, dist)),
				sphere->position));
	if ((int)((floor((0.5f + atan2f(diff.z, diff.x) / (2.0f * M_PI)) * 10.0f))
		+ (floor((0.5f - asinf(diff.y) / M_PI) * 10.0f))) % 2 == 0)
		return (0xFFFFFF);
	else
		return (0x000000);
}

static inline float	get_intensity(t_ray *ray, t_ambiant *ambiant,
	t_sphere *sphere, float dist)
{
	float		intensity;
	float		level;
	t_fvector3	direction;

	level = ambiant->level;
	direction = ray->direction;
	intensity = level + (1 - level)
		* fmax(ft_fdot_product(ft_fnormalize(
					ft_fvector3_diff(ft_fvector3_sum(ray->origin,
							ft_fvector3_scale(direction, dist)),
						sphere->position)),
				ft_fnormalize(ft_fvector3_scale(direction, -1.0f))), 0.0f);
	if (intensity > 1.0f)
		intensity = 1.0f;
	return (intensity);
}
