/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:07:44 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/20 20:04:54 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline float	intersection_sphere(t_ray ray, t_sphere *sphere);
static float	get_intensity(t_ray *ray, t_ambiant *ambiant, t_sphere *sphere,
						float dist);
/* -------------------------------------------------------------------------- */

t_sphere	*sphere(t_fvector3 position, float diameter, t_rgb color)
{
	t_sphere	*sp;

	sp = malloc(sizeof(t_sphere));
	if (!sp)
		return (NULL);
	sp->id = SPHERE_ID;
	sp->position = position;
	sp->diameter = diameter;
	sp->radius = diameter / 2.0f;
	sp->color = color;
	sp->render = get_render_by_id(SPHERE_ID);
	return (sp);
}

void	*parse_sphere(char **values)
{
	t_fvector3	position;
	float		diameter;
	t_rgb		color;

	if (!values[0] || !values[1] || !values[2] || values[3])
		return (error_and_null(SP_ARGS_E));
	if (!parse_fvector3(values[0], &position, SP_POS_E))
		return (NULL);
	if (!ft_isnumeric(values[1]) || ft_isoutint(values[1]))
		return (error_and_null(SP_DIAM_E));
	diameter = ft_atof(values[1]);
	if (diameter < 0.0f)
		return (error_and_null(SP_DIAM_E));
	if (!parse_color(values[2], &color, SP_RGB_E))
		return (NULL);
	return (sphere(position, diameter, color));
}

void	render_sphere(t_minirt *mrt, t_ray *ray, t_object *object)
{
	t_sphere	*sphere;
	float		dist;
	float		intensity;
	t_fvector3	impact_point;

	sphere = (t_sphere *)object;
	dist = intersection_sphere(*ray, sphere);
	if (dist > 0 && dist <= ray->dist)
	{
		impact_point = ft_fvector3_sum(
				ray->origin,
				ft_fvector3_scale(ray->direction, dist)
				);
		intensity = get_intensity(ray, mrt->ambiant, sphere, dist);
		ray->color = calculate_brightness(
				mrt,
				impact_point,
				ft_fnormalize(ft_fvector3_diff(impact_point, sphere->position)),
				sphere->color
				);
		ray->color.r *= intensity;
		ray->color.g *= intensity;
		ray->color.b *= intensity;
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

static float	get_intensity(t_ray *ray, t_ambiant *ambiant, t_sphere *sphere,
	float dist)
{
	float		intensity;
	float		level;
	t_fvector3	direction;

	level = ambiant->level;
	direction = ray->direction;
	intensity = level + (1 - level) * fmax(ft_fdot_product(
				ft_fnormalize(ft_fvector3_diff(
						ft_fvector3_sum(
							ray->origin,
							ft_fvector3_scale(direction, dist)),
						sphere->position)),
				ft_fnormalize(ft_fvector3_scale(direction, -1.0f))), 0.0f);
	if (intensity > 1.0f)
		intensity = 1.0f;
	return (intensity);
}
