/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:10:17 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/17 10:06:58 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_plane	*plane(t_fvector3 position, t_fvector3 normal, t_rgb color)
{
	t_plane	*pl;

	pl = malloc(sizeof(t_plane));
	if (!pl)
		return (NULL);
	pl->id = PLANE_ID;
	pl->position = position;
	pl->normal = normal;
	pl->color = color;
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

// t = ((P - O) * N) / D * N
// P = point du plan.		O = origin du rayon.
// D = direction du rayon.	N = normal du plan.
static float	intersection_plane(t_ray ray, t_plane *plane)
{
	t_fvector3		vec;
	float			denominator;
	float			t;

	denominator = dot(ray.direction, plane->normal);
	if (fabs(denominator) < 0.000001)
		return (-1.0f);
	vec = sub_vectors(plane->position, ray.origin);
	t = dot(vec, plane->normal) / denominator;
	if (t >= 0)
		return (t);
	return (-1.0f);
}

void	render_plane(t_ray ray, t_fvector2 pixelpos, t_object *object)
{
	float	t;
	t_plane	*plane;

	plane = (t_plane *)object;
	t = intersection_plane(ray, plane);
	if (t > 0)
		put_pixel(minirt()->mlx, pixelpos, plane->color);
}
