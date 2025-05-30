/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:04:39 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/30 12:16:15 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline float	intersect_light(t_ray *ray, t_light *light,
						float amplifier);
static inline float	get_intensity(t_ray *ray, t_light *light, float dist);
/* -------------------------------------------------------------------------- */

t_light	*light(t_fvector3 position, float level, t_rgb color, float scale)
{
	t_light	*l;

	l = malloc(sizeof(t_light));
	if (!l)
		return (NULL);
	l->id = LIGHT_ID;
	l->position = position;
	l->level = level;
	l->color = color;
	l->scale = scale;
	l->methods = get_methods_by_id(LIGHT_ID);
	l->selected = 0;
	return (l);
}

void	*parse_light(char **values)
{
	t_fvector3	position;
	float		level;
	t_rgb		color;
	float		scale;

	if (!values[0] || !values[1] || !values[2] || (values[3] && values[4]))
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
	scale = 0.0f;
	if (values[3])
	{
		scale = ft_atof(values[3]);
		if (!ft_isnumeric(values[3]) || scale < 0.0f)
			return (error_and_null(L_SCALE_E));
	}
	return (light(position, level, color, scale));
}

void	show_light(t_ray *ray, t_light *light)
{
	float		dist;
	float		intensity;

	dist = intersect_light(ray, light, 1.0f);
	if (dist > 0.0f && dist <= ray->dist)
	{
		ray->color.r += (light->color.r - ray->color.r);
		ray->color.g += (light->color.g - ray->color.g);
		ray->color.b += (light->color.b - ray->color.b);
	}
	else
	{
		dist = intersect_light(ray, light, 6.0f * light->scale);
		if (dist > 0.0f && dist <= ray->dist)
		{
			intensity = get_intensity(ray, light, dist);
			ray->color.r += (intensity * (light->color.r - ray->color.r));
			ray->color.g += (intensity * (light->color.g - ray->color.g));
			ray->color.b += (intensity * (light->color.b - ray->color.b));
		}
	}
}

static inline float	intersect_light(t_ray *ray, t_light *light, float amplifier)
{
	t_fvector3	oc;
	float		b;
	float		delta;
	float		x;
	float		radius;

	if (light->scale == 0.0f)
		return (-1.0f);
	oc = ft_fvector3_diff(ray->origin, light->position);
	b = 2.0f * ft_fdot_product(oc, ray->direction);
	radius = light->level * light->scale;
	delta = b * b - 4.0f * (ft_fdot_product(oc, oc)
			- (radius * radius * amplifier));
	if (delta < 0.0f)
		return (-1.0f);
	x = (-b - sqrtf(delta)) / 2.0f;
	if (x > EPSILON)
		return (x);
	x = (-b + sqrtf(delta)) / 2.0f;
	if (x > EPSILON)
		return (x);
	return (-1.0f);
}

static inline float	get_intensity(t_ray *ray, t_light *light, float dist)
{
	float		intensity;
	t_fvector3	direction;

	direction = ray->direction;
	intensity = fmax(ft_fdot_product(
				ft_fnormalize(ft_fvector3_diff(
						ft_fvector3_sum(ray->origin, ft_fvector3_scale(
								direction, dist)), light->position
						)),
				ft_fnormalize(ft_fvector3_scale(direction, -1.0f))
				),
			0.0f
			);
	intensity = powf(intensity, 3.0f);
	if (intensity > 1.0f)
		intensity = 1.0f;
	return (intensity);
}
