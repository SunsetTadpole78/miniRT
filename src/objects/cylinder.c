/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:11:35 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/19 18:15:20 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_cylinder	*cylinder(t_fvector3 position, t_fvector3 normal,
	t_fvector2 size, t_rgb color)
{
	t_cylinder	*cy;

	cy = malloc(sizeof(t_cylinder));
	if (!cy)
		return (NULL);
	cy->id = CYLINDER_ID;
	cy->position = position;
	cy->normal = normal;
	cy->size = size;
	cy->color = color;
	return (cy);
}

void	*parse_cylinder(char **values)
{
	t_fvector3	position;
	t_fvector3	normal;
	t_fvector2	size;
	t_rgb		color;

	if (!values[0] || !values[1] || !values[2] || !values[3]
		|| !values[4] || values[5])
		return (error_and_null(CY_ARGS_E));
	if (!parse_fvector3(values[0], &position, CY_POS_E)
		|| !parse_normal(values[1], &normal, CY_NORM_E))
		return (NULL);
	if (!ft_isnumeric(values[2]) || ft_isoutint(values[2]))
		return (error_and_null(CY_DIAM_E));
	size.x = ft_atof(values[2]);
	if (size.x < 0.0f)
		return (error_and_null(CY_DIAM_E));
	if (!ft_isnumeric(values[3]) || ft_isoutint(values[3]))
		return (error_and_null(CY_HEI_E));
	size.y = ft_atof(values[3]);
	if (size.y < 0.0f)
		return (error_and_null(CY_HEI_E));
	if (!parse_color(values[4], &color, CY_RGB_E))
		return (NULL);
	return (cylinder(position, normal, size, color));
}

float	intersect_cap(t_ray ray, t_fvector3 center, float radius, float y_plane)
{
	float		t;
	float		dx;
	float		dz;
	t_fvector3	p;

	if (fabs(ray.direction.y) < 1e-6)
		return (-1);
	t = (y_plane - ray.origin.y) / ray.direction.y;
	if (t < 0.001f)
		return (-1);
	p = ft_fvector3_sum(ray.origin, ft_fvector3_scale(ray.direction, t));
	dx = p.x - center.x;
	dz = p.z - center.z;
	if ((dx * dx + dz * dz) <= (radius * radius))
		return (t);
	return (-1);
}

float	intersection_cylinder(t_ray ray, t_cylinder *cylinder)
{
	t_fvector3	oc;
	float		a;
	float		b;
	float		c;
	float		x1;
	float		x2;
	float		delta;
	float		t;
	float		v;
	float		y;
	float		b1;
	float		b2;
	float		t_cap1;
	float		t_cap2;

	oc = ft_fvector3_diff(ray.origin, cylinder->position);
	a = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z;
	b = 2 * (oc.x * ray.direction.x + oc.z * ray.direction.z);
	c = oc.x * oc.x + oc.z * oc.z - ((cylinder->size.x / 2)
			* (cylinder->size.x / 2));
	delta = b * b - 4 * a * c;
	if (delta < 0)
		return (-1.0f);
	x1 = (-b - sqrtf(delta)) / (2.0f * a);
	x2 = (-b + sqrtf(delta)) / (2.0f * a);
	v = -1;
	if (x1 > 0.001f)
		v = x1;
	else if (x2 > 0.001f)
		v = x2;
	t = -1;
	if (v > 0.001f)
	{
		y = ray.origin.y + v * ray.direction.y;
		b1 = cylinder->position.y - (cylinder->size.y / 2);
		b2 = cylinder->position.y + (cylinder->size.y / 2);
		if (y >= b1 && y <= b2)
			t = v;
	}
	t_cap1 = intersect_cap(ray, cylinder->position, cylinder->size.x / 2,
			cylinder->position.y - cylinder->size.y / 2);
	t_cap2 = intersect_cap(ray, cylinder->position, cylinder->size.x / 2,
			cylinder->position.y + cylinder->size.y / 2);
	if (t_cap1 > 0.001f && (t < 0 || t_cap1 < t))
		t = t_cap1;
	if (t_cap2 > 0.001f && (t < 0 || t_cap2 < t))
		t = t_cap2;
	return (t);
}

void	render_cylinder(t_mlx *mlx, t_ray *ray, t_fvector2 pixel,
	t_object *object)
{
	float		dist;
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *)object;
	dist = intersection_cylinder(*ray, cylinder);
	if (dist > 0 && dist <= ray->dist)
	{
		put_pixel(mlx, pixel, cylinder->color);
		ray->dist = dist;
	}
}
