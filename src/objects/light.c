/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:04:39 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/30 00:29:17 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline float	intersection_billboard(t_ray ray, t_light *light,
						float *intensity);
/* -------------------------------------------------------------------------- */

t_light	*light(t_fvector3 position, float level, t_rgb color)
{
	t_light	*l;

	l = malloc(sizeof(t_light));
	if (!l)
		return (NULL);
	l->id = LIGHT_ID;
	l->position = position;
	l->level = level;
	l->color = color;
	l->methods = get_methods_by_id(LIGHT_ID);
	l->selected = 0;
	return (l);
}

void	*parse_light(char **values)
{
	t_fvector3	position;
	float		level;
	t_rgb		color;

	if (!values[0] || !values[1] || !values[2] || values[3])
		return (error_and_null(L_ARGS_E));
	if (!parse_fvector3(values[0], &position))
		return (error_and_null(L_POS_E));
	if (!ft_isnumeric(values[1]))
		return (error_and_null(L_LVL_E));
	level = ft_atof(values[1]);
	if (level < 0.0f || level > 1.0f)
		return (error_and_null(L_LVL_E));
	if (!parse_color(values[2], &color))
		return (error_and_null(L_RGB_E));
	return (light(position, level, color));
}

void	show_light(t_ray *ray, t_light *light)
{
	float		dist;
	float		intensity;

	dist = intersection_billboard(*ray, light, &intensity);
	if (dist > 0 && dist <= ray->dist)
	{
		ray->color.r += (intensity * (light->color.r - ray->color.r));
		ray->color.g += (intensity * (light->color.g - ray->color.g));
		ray->color.b += (intensity * (light->color.b - ray->color.b));
	}
}

static inline int	calcul_intensity(t_light *light, float *intensity, float d)
{
	float	inner_radius;
	float	outer_radius;

	inner_radius = light->level;
	outer_radius = 4.0f * light->level;
	if (d <= inner_radius)
		*intensity = 1;
	else if (d <= outer_radius)
		*intensity = powf(1.0f
				- ((d - inner_radius) / (outer_radius - inner_radius)), 2);
	else
		return (0);
	return (1);
}

static inline float	intersection_billboard(t_ray ray, t_light *light,
		float *intensity)
{
	t_fvector3	normal;
	float		denominator;
	float		t;
	t_fvector3	diff;
	float		d;

	normal = ft_fnormalize(ft_fvector3_diff(light->position, ray.origin));
	denominator = ft_fdot_product(ray.direction, normal);
	if (fabs(denominator) < EPSILON)
		return (-1.0f);
	t = ft_fdot_product(ft_fvector3_diff(light->position, ray.origin), normal)
		/ denominator;
	if (t < EPSILON)
		return (-1.0f);
	diff = ft_fvector3_diff(ft_fvector3_sum(ray.origin,
				ft_fvector3_scale(ray.direction, t)), light->position);
	d = ft_fvector3_length(ft_fvector3_diff(diff, ft_fvector3_scale(normal,
					ft_fdot_product(diff, normal))));
	if (!calcul_intensity(light, intensity, d))
		return (-1.0f);
	return (t);
}
