/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:07:44 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/17 10:06:41 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

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

// discriminant = b^2 - 4c
// t1 = (-b - sqrt(discriminant)) / 2.0f;
// t2 = (-b + sqrt(discriminant)) / 2.0f;
static float	intersection_sphere(t_ray ray, t_sphere *sphere)
{
	t_fvector3		oc;
	float			b;
	float			discriminant;
	float			t1;
	float			t2;

	oc = sub_vectors(ray.origin, sphere->position);
	b = 2.0f * dot(oc, ray.direction);
	discriminant = b * b - 4.0f
		* (dot(oc, oc) - (sphere->radius * sphere->radius));
	if (discriminant < 0)
		return (-1.0f);
	t1 = (-b - sqrtf(discriminant)) / 2.0f;
	t2 = (-b + sqrtf(discriminant)) / 2.0f;
	if (t1 > 0.001f)
		return (t1);
	if (t2 > 0.001f)
		return (t2);
	return (-1.0f);
}

void	render_sphere(t_ray *ray, t_fvector2 pixelpos, t_object *object)
{
	float		t;
	t_sphere	*sphere;

	sphere = (t_sphere *)object;
	t = intersection_sphere(*ray, sphere);
	if (t > 0 && t <= ray->t)
	{
		put_pixel(minirt()->mlx, pixelpos, sphere->color);
		ray->t = t;
	}
}
