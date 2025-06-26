/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 01:38:41 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/26 19:31:47 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static inline void	check_intersection(t_object *object, t_ray *ray);

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
	t_light		*light;

	if (depth > MAX_DEPTH)
		return (ray->color);
	ray->dist = 3.4E+38;
	cur = mrt->objects;
	while (cur)
	{
		check_intersection(cur, ray);
		cur = cur->next;
	}
	ray->color = (t_rgb){0, 0, 0};
	if (ray->dist < 3.4E+37)
		ray->object->methods->apply_lights(mrt, ray, ray->object, depth);
	light = mrt->lights;
	while (light)
	{
		if (light->visible)
			show_light(ray, light);
		light = (t_light *)light->next;
	}
	return (ray->color);
}

static inline void	check_intersection(t_object *object, t_ray *ray)
{
	float	(*intersect)(t_ray *, t_object *, float);
	float	dist;

	intersect = object->methods->intersect;
	if (intersect)
	{
		dist = intersect(ray, object, 1.0f);
		if (dist > 0 && dist <= ray->dist)
		{
			ray->object = object;
			ray->dist = dist;
		}
	}
}
