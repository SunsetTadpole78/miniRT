/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 01:38:41 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 18:52:32 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_fvector3	primary_ray(t_camera *cam, t_vector2 pos, float ratio)
{
	t_fvector3	ndc_vec;

	ndc_vec = (t_fvector3){
		-(2.0f * (((float)pos.x + 0.5f) / WIN_WIDTH) - 1.0f) * ratio,
		-(2.0f * (((float)pos.y + 0.5f) / WIN_HEIGHT) - 1.0f)
		* cam->iplane_scale, 1.0f};
	return (ft_fnormalize(
			ft_fvector3_sum(
				ft_fvector3_sum(
					(t_fvector3){cam->right.x * ndc_vec.x,
					cam->right.y * ndc_vec.x, cam->right.z * ndc_vec.x},
				(t_fvector3){cam->up.x * ndc_vec.y,
				cam->up.y * ndc_vec.y, cam->up.z * ndc_vec.y}),
		cam->normal)));
}

t_rgb	ray_tracer(t_minirt *mrt, t_ray *ray, int depth)
{
	t_object	*cur;
	float			(*intersect)(t_ray *, t_object *, float);
	t_light		*light;
	float		dist;

	if (depth > MAX_DEPTH)
		return (ray->color);
	ray->dist = 3.4E+38;
	cur = mrt->objects;
	while (cur)
	{
		intersect = cur->methods->intersect;
		if (intersect)
		{
			dist = intersect(ray, cur, 1.0f);
			if (dist > 0 && dist <= ray->dist)
			{
				ray->object = cur;
				ray->dist = dist;
			}
		}
		cur = cur->next;
	}
	if (ray->dist >= 3.4E+37)
		ray->color = (t_rgb){0, 0, 0};
	else
		ray->object->methods->apply_lights(mrt, ray, ray->object, 1.0f);
	light = mrt->lights;
	while (light)
	{
		if (light->visible)
			show_light(ray, light);
		light = (t_light *)light->next;
	}
	return (ray->color);
}
