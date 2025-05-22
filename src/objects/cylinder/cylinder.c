/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:11:35 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/22 19:31:50 by lroussel         ###   ########.fr       */
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
	cy->radius = size.x / 2.0f;
	cy->half_height = size.y / 2.0f;
	cy->render = get_render_by_id(CYLINDER_ID);
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

float	intersection_cylinder(t_ray ray, t_cylinder *cylinder)
{
	t_fvector3	local_origin;
	t_fvector3	local_dir;
	float		t;
	float		t_cap_bottom;
	float		t_cap_top;

	normalize_side(&local_origin, &local_dir, ray, cylinder);
	t = apply_side_equation(local_origin, local_dir, cylinder);
	t_cap_bottom = intersect_cap(local_origin, local_dir, cylinder->radius,
			-(cylinder->half_height));
	t_cap_top = intersect_cap(local_origin, local_dir, cylinder->radius,
			cylinder->half_height);
	if (t_cap_bottom > 0.001f && (t < 0 || t_cap_bottom < t))
		t = t_cap_bottom;
	if (t_cap_top > 0.001f && (t < 0 || t_cap_top < t))
		t = t_cap_top;
	return (t);
}

void	render_cylinder(t_minirt *mrt, t_ray *ray, t_object *object)
{
	float		dist;
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *)object;
	dist = intersection_cylinder(*ray, cylinder);
	if (dist > 0 && dist <= ray->dist)
	{
		(void)mrt;
		ray->color = cylinder->color;
		ray->dist = dist;
	}
}
