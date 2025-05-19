/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:11:35 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/20 00:47:05 by lroussel         ###   ########.fr       */
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

float	intersect_cap(t_fvector3 local_origin, t_fvector3 local_dir,
	float radius, float half_height)
{
	float	t;
	float	x;
	float	z;

	if (fabs(local_dir.y) < 1e-6)
		return (-1);
	t = (half_height - local_origin.y) / local_dir.y;
	if (t < 0.001f)
		return (-1);
	x = local_origin.x + t * local_dir.x;
	z = local_origin.z + t * local_dir.z;
	if ((x * x + z * z) <= (radius * radius))
		return (t);
	return (-1);
}

float	intersection_cylinder(t_ray ray, t_cylinder *cylinder)
{
	t_fvector3	oc;
	float		a;
	float		b;
	float		c;
	float		delta;
	float		t;
	float		t_cap1;
	float		t_cap2;
	float		half_height;
	t_fvector3	up;
	t_fvector3	right;
	t_fvector3	forward;
	float		sqrt_delta;
	float		t1;
	float		t2;
	float		t_candidate;
	float		y_hit;
	t_fvector3	local_origin;
	t_fvector3	local_dir;

	half_height = cylinder->size.y / 2.0f;
	oc = ft_fvector3_diff(ray.origin, cylinder->position);
	up = ft_fnormalize(cylinder->normal);
	if (fabsf(up.y) < 0.999f)
		right = ft_fnormalize(
				ft_fcross_product(ft_fvector3(0.0f, 1.0f, 0.0f), up));
	else
		right = ft_fnormalize(
				ft_fcross_product(ft_fvector3(1.0f, 0.0f, 0.0f), up));
	forward = ft_fcross_product(up, right);
	local_origin = (t_fvector3){
		ft_fdot_product(oc, right),
		ft_fdot_product(oc, up),
		ft_fdot_product(oc, forward)
	};
	local_dir = ft_fvector3(
			ft_fdot_product(ray.direction, right),
			ft_fdot_product(ray.direction, up),
			ft_fdot_product(ray.direction, forward)
			);
	a = local_dir.x * local_dir.x + local_dir.z * local_dir.z;
	b = 2 * (local_origin.x * local_dir.x + local_origin.z * local_dir.z);
	c = local_origin.x * local_origin.x + local_origin.z * local_origin.z
		- (cylinder->size.x / 2) * (cylinder->size.x / 2);
	delta = b * b - 4 * a * c;
	t = -1;
	if (delta >= 0.0f)
	{
		sqrt_delta = sqrtf(delta);
		t1 = (-b - sqrt_delta) / (2.0f * a);
		t2 = (-b + sqrt_delta) / (2.0f * a);
		if (t1 > 0.001f)
			t_candidate = t1;
		else if (t2 > 0.001f)
			t_candidate = t2;
		else
			t_candidate = -1.0f;
		if (t_candidate > 0.001f)
		{
			y_hit = local_origin.y + t_candidate * local_dir.y;
			if (y_hit >= -half_height && y_hit <= half_height)
				t = t_candidate;
		}
	}
	t_cap1 = intersect_cap(local_origin, local_dir, cylinder->size.x / 2,
			-(cylinder->size.y / 2));
	t_cap2 = intersect_cap(local_origin, local_dir, cylinder->size.x / 2,
			cylinder->size.y / 2);
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
