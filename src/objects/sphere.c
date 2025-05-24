/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:07:44 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/24 18:56:16 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline float	intersection_sphere(t_ray ray, t_sphere *sphere);
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
	int			inside;
	t_hit_data	hit;

	sphere = (t_sphere *)object;
	dist = intersection_sphere(*ray, sphere);
	if (dist > 0 && dist <= ray->dist)
	{
		hit.object = object;
		hit.impact_point = ft_fvector3_sum(ray->origin,
				ft_fvector3_scale(ray->direction, dist));
		hit.normal = ft_fnormalize(ft_fvector3_diff(hit.impact_point,
					sphere->position));
		hit.position = sphere->position;
		inside = ft_fvector3_length(ft_fvector3_diff(ray->origin,
					sphere->position)) < sphere->radius;
		if (inside)
			hit.normal = ft_fvector3_scale(hit.normal, -1);
		ray->color = apply_lights_modifier(
				get_lights_modifier(mrt, hit, inside, is_inside_sphere),
				sphere->color);
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

int	is_inside_sphere(t_hit_data hit, t_fvector3 point)
{
	t_sphere	*sphere;

	sphere = (t_sphere *)hit.object;
	return (ft_fvector3_length(ft_fvector3_diff(point,
				hit.position)) < sphere->radius);
}
