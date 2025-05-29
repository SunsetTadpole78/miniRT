/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:11:35 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/29 15:16:23 by lroussel         ###   ########.fr       */
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
	cy->render = get_render_by_id(CYLINDER_ID);
	cy->intersect = get_intersect_by_id(CYLINDER_ID);
	cy->is_inside = is_inside_cylinder;
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
	t_ray		reflect_ray;

	cylinder = (t_cylinder *)object;
	dist = intersect_cylinder(ray, object, 1.0f);
	if (dist < 0.0f || dist > ray->dist)
		return ;
	hit.object = object;
	hit.position = cylinder->position;
	hit.impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, dist));
	hit.normal = get_normal(ray->extra, hit.impact_point, cylinder);
	inside = is_inside_cylinder(object, ray->origin);
	if (inside)
		hit.normal = ft_fvector3_scale(hit.normal, -1);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			cylinder->pattern.main_color);
	reflect_ray = *ray;
	specular_reflection(&reflect_ray, &hit, cylinder->pattern.smoothness);
	ray->color = ft_rgb_lerp(ray->color, ray_tracer(mrt, &reflect_ray,
				depth + 1), cylinder->pattern.mattifying);
	ray->dist = dist;
}

float	intersect_cylinder(t_ray *ray, t_object *object, float amplifier)
{
	t_cylinder	*cylinder;
	t_fvector3	local_origin;
	t_fvector3	local_dir;
	t_fvector3	t;
	t_fvector2	amplified;

	cylinder = (t_cylinder *)object;
	normalize_side(&local_origin, &local_dir, *ray, cylinder);
	t.z = apply_side_equation(local_origin, local_dir, cylinder, amplifier);
	if (fabsf(local_dir.y) < 1e-6f)
		return (t.z);
	amplified = (t_fvector2){cylinder->radius * amplifier,
		cylinder->half_height * amplifier};
	t.x = intersect_cap(local_origin, local_dir, amplified.x, -amplified.y);
	t.y = intersect_cap(local_origin, local_dir, amplified.x, amplified.y);
	ray->extra = t.x > EPSILON && (t.z < 0 || t.x < t.z);
	if (ray->extra == 1)
		t.z = t.x;
	if (t.y > EPSILON && (t.z < 0 || t.y < t.z))
	{
		ray->extra = 2;
		t.z = t.y;
	}
	return (t.z);
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
