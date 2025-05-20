/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:04:39 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/20 18:42:42 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_light	*light(t_fvector3 position, float level, t_rgb color)
{
	t_light	*l;

	l = malloc(sizeof(t_light));
	if (!l)
		return (NULL);
	l->id = LIGHT_ID;
	l->position = position;
	l->level = level;
	l->color = color;
	l->render = get_render_by_id(LIGHT_ID);
	l->linear_at_coef = 0.008f / level;
	l->quadratic_at_coef = 0.0008f / level;
	color.r += (255 - color.r) * (1 - level);
	color.g += (255 - color.g) * (1 - level);
	color.b += (255 - color.b) * (1 - level);
	l->render_color = color;
	return (l);
}

void	*parse_light(char **values)
{
	t_fvector3	position;
	float		level;
	t_rgb		color;

	if (!values[0] || !values[1] || !values[2] || values[3])
		return (error_and_null(L_ARGS_E));
	if (!parse_fvector3(values[0], &position, L_POS_E))
		return (NULL);
	level = ft_atof(values[1]);
	if (level < 0.0f || level > 1.0f)
		return (error_and_null(L_LVL_E));
	if (!parse_color(values[2], &color, L_RGB_E))
		return (NULL);
	return (light(position, level, color));
}

static float	intersection_light(t_ray ray, t_light *light)
{
	t_fvector3		oc;
	float			b;
	float			delta;
	float			x1;
	float			x2;

	oc = ft_fvector3_diff(ray.origin, light->position);
	b = 2.0f * ft_fdot_product(oc, ray.direction);
	delta = b * b - 4.0f
		* (ft_fdot_product(oc, oc) - 0.5f);
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

void	render_light(t_minirt *mrt, t_ray *ray, t_object *object)
{
	float		dist;
	t_light	*light;

	(void)mrt;
	light = (t_light *)object;
	dist = intersection_light(*ray, light);
	if (dist > 0 && dist <= ray->dist)
	{
		ray->color = light->render_color;
		ray->dist = dist;
	}
}
