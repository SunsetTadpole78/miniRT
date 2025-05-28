/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:11:35 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/28 17:38:18 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_fvector3	get_normal(int type, t_fvector3 impact_point,
								t_cylinder *cylinder);
/* -------------------------------------------------------------------------- */

t_cylinder	*cylinder(t_fvector3 position, t_fvector3 normal,
	t_fvector2 size, t_pattern pattern)
{
	t_cylinder	*cy;

	cy = malloc(sizeof(t_cylinder));
	if (!cy)
		return (NULL);
	cy->id = CYLINDER_ID;
	cy->position = position;
	cy->normal = normal;
	cy->pattern = pattern;
	cy->diameter = size.x;
	cy->radius = size.x / 2.0f;
	cy->height = size.y;
	cy->half_height = size.y / 2.0f;
	cy->methods = get_methods_by_id(CYLINDER_ID);
	return (cy);
}

void	*parse_cylinder(char **values)
{
	t_fvector3	position;
	t_fvector3	normal;
	t_fvector2	size;
	t_pattern	pattern;

	if (!values[0] || !values[1] || !values[2] || !values[3] || !values[4]
		|| (values[5] && values[6] && values[7] && values[8] && values[9]))
		return (error_and_null(CY_ARGS_E));
	if (!parse_fvector3(values[0], &position))
		return (error_and_null(CY_POS_E));
	if (!parse_normal(values[1], &normal))
		return (error_and_null(CY_NORM_E));
	size.x = ft_atof(values[2]);
	if (!ft_isnumeric(values[2]) || ft_isoutint(values[2]) || size.x < 0.0f)
		return (error_and_null(CY_DIAM_E));
	size.y = ft_atof(values[3]);
	if (!ft_isnumeric(values[3]) || ft_isoutint(values[3]) || size.y < 0.0f)
		return (error_and_null(CY_HEI_E));
	if (!parse_color(values[4], &pattern.main_color))
		return (error_and_null(CY_RGB_E));
	init_pattern(&pattern);
	if (values[5] && !parse_pattern(values + 5, &pattern))
		return (error_and_null(CY_ARGS_E));
	return (cylinder(position, normal, size, pattern));
}

void	render_cylinder(t_minirt *mrt, t_ray *ray, t_object *object, int depth)
{
	float		dist;
	t_cylinder	*cylinder;
	t_hit_data	hit;
	int			inside;

	(void)depth;
	cylinder = (t_cylinder *)object;
	dist = intersect_cylinder(*ray, object);
	if (dist < 0.0f || dist > ray->dist)
		return ;
	hit.object = object;
	hit.position = cylinder->position;
	hit.impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, dist));
	hit.normal = get_normal(cylinder->type, hit.impact_point, cylinder);
	inside = is_inside_cylinder(hit, ray->origin);
	if (inside)
		hit.normal = ft_fvector3_scale(hit.normal, -1);
	ray->color = apply_lights_modifier(
			get_lights_modifier(mrt, hit, inside, is_inside_cylinder),
			cylinder->pattern.main_color);
	ray->dist = dist;
}

float	intersect_cylinder(t_ray ray, t_object *object)
{
	t_cylinder	*cylinder;
	t_fvector3	local_origin;
	t_fvector3	local_dir;
	float		t;
	t_fvector2	caps_t;

	cylinder = (t_cylinder *)object;
	normalize_side(&local_origin, &local_dir, ray, cylinder);
	t = apply_side_equation(local_origin, local_dir, cylinder);
	if (fabsf(local_dir.y) < 1e-6f)
		return (t);
	caps_t.x = intersect_cap(local_origin, local_dir, cylinder->radius,
			-(cylinder->half_height));
	caps_t.y = intersect_cap(local_origin, local_dir, cylinder->radius,
			cylinder->half_height);
	cylinder->type = caps_t.x > EPSILON && (t < 0 || caps_t.x < t);
	if (cylinder->type == 1)
		t = caps_t.x;
	if (caps_t.y > EPSILON && (t < 0 || caps_t.y < t))
	{
		cylinder->type = 2;
		t = caps_t.y;
	}
	return (t);
}

static inline t_fvector3	get_normal(int type, t_fvector3 impact_point,
		t_cylinder *cylinder)
{
	if (type == 1)
		return (ft_fvector3_scale(cylinder->normal, -1.0f));
	if (type == 2)
		return (cylinder->normal);
	return (ft_fnormalize(ft_fvector3_diff(impact_point,
				ft_fvector3_sum(cylinder->position,
					ft_fvector3_scale(cylinder->normal,
						ft_fdot_product(ft_fvector3_diff(
								impact_point,
								cylinder->position),
							cylinder->normal))))));
}
