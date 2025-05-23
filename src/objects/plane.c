/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:10:17 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/22 15:17:51 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline float	intersection_plane(t_ray ray, t_plane *plane);
/* -------------------------------------------------------------------------- */

t_plane	*plane(t_fvector3 position, t_fvector3 normal, t_rgb color)
{
	t_plane	*pl;

	pl = malloc(sizeof(t_plane));
	if (!pl)
		return (NULL);
	pl->id = PLANE_ID;
	pl->position = position;
	pl->normal = ft_fnormalize(normal);
	pl->color = color;
	pl->render = get_render_by_id(PLANE_ID);
	return (pl);
}

void	*parse_plane(char **values)
{
	t_fvector3	position;
	t_fvector3	normal;
	t_rgb		color;

	if (!values[0] || !values[1] || !values[2] || values[3])
		return (error_and_null(PL_ARGS_E));
	if (!parse_fvector3(values[0], &position, PL_POS_E)
		|| !parse_normal(values[1], &normal, PL_NORM_E)
		|| !parse_color(values[2], &color, PL_RGB_E))
		return (NULL);
	return (plane(position, normal, color));
}

void	render_plane(t_minirt *mrt, t_ray *ray, t_object *object)
{
	t_plane		*plane;
	float		dist;
	t_hit_data	hit;

	plane = (t_plane *)object;
	dist = intersection_plane(*ray, plane);
	if (dist > 0 && dist <= ray->dist)
	{
		hit.position = plane->position;
		hit.impact_point = ft_fvector3_sum(ray->origin,
				ft_fvector3_scale(ray->direction, dist));
		hit.normal = plane->normal;
		if (ft_fdot_product(ray->direction, hit.normal) > 0)
			hit.normal = ft_fvector3_scale(hit.normal, -1);
		ray->color = apply_lights_modifier(
				get_lights_modifier(mrt, hit, 0),
				plane->color);
		ray->dist = dist;
	}
}

// t = ((P - O) * N) / D * N
// P = point du plan.		O = origin du rayon.
// D = direction du rayon.	N = normal du plan.
static inline float	intersection_plane(t_ray ray, t_plane *plane)
{
	float	denominator;
	float	x;

	denominator = ft_fdot_product(ray.direction, plane->normal);
	if (fabs(denominator) < 0.000001f)
		return (-1.0f);
	x = ft_fdot_product(ft_fvector3_diff(plane->position, ray.origin),
			plane->normal) / denominator;
	if (x >= 0.0f)
		return (x);
	return (-1.0f);
}
