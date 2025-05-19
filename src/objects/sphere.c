/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:07:44 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/19 11:50:26 by lroussel         ###   ########.fr       */
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

// discriminant = b^2 - 4c
// t1 = (-b - sqrt(discriminant)) / 2.0f;
// t2 = (-b + sqrt(discriminant)) / 2.0f;
static float	intersection_sphere(t_ray ray, t_sphere *sphere)
{
	t_fvector3		oc;
	float			b;
	float			delta;
	float			x1;
	float			x2;

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
	t_fvector3	hit_point;
	t_fvector3	normal;
	t_fvector3	view_dir;
	float		view_factor;
	float		intensity;

	hit_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, dist));
	normal = ft_fnormalize(ft_fvector3_diff(hit_point, sphere->position));
	view_dir = ft_fnormalize(ft_fvector3_scale(ray->direction, -1.0f));
	view_factor = fmax(ft_fdot_product(normal, view_dir), 0.0f);
	intensity = (ambiant->level + (1 - ambiant->level) * view_factor);
	if (intensity > 1.0f)
		intensity = 1.0f;
	return (intensity);
}

void	render_sphere(t_minirt *mrt, t_ray *ray,
		t_vector2 pixel, t_object *object)
{
	float		dist;
	t_sphere	*sphere;
	t_mlx		*mlx;
	t_rgb		color;
	float		intensity;

	sphere = (t_sphere *)object;
	dist = intersection_sphere(*ray, sphere);
	mlx = mrt->mlx;
	if (dist > 0 && dist <= ray->dist)
	{
		intensity = get_intensity(ray, mrt->ambiant, sphere, dist);
		color.r = sphere->color.r * intensity;
		color.g = sphere->color.g * intensity;
		color.b = sphere->color.b * intensity;
		put_pixel(mlx, pixel, color);
		ray->dist = dist;
	}
}
