/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   click.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/30 16:07:54 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void	init_ray(t_ray *ray, t_minirt *mrt, t_vector2 pos);
static inline void	check_object(t_object *object, t_ray *ray,
						float (*intersect)(t_ray *, t_object *, float),
						t_minirt *mrt);
/* -------------------------------------------------------------------------- */

int	on_click(int id, int x, int y, t_minirt *mrt)
{
	t_object	*cur;
	t_ray		ray;
	float		(*intersect)(t_ray *, t_object *, float);

	if (id != 1)
		return (0);
	cur = mrt->objects;
	init_ray(&ray, mrt, (t_vector2){x, y});
	while (cur)
	{
		intersect = cur->methods->intersect;
		if (intersect)
			check_object(cur, &ray, intersect, mrt);
		cur = cur->next;
	}
	cur = (t_object *)mrt->lights;
	while (cur)
	{
		check_object(cur, &ray, intersect_light, mrt);
		cur = cur->next;
	}
	return (0);
}

static inline void	init_ray(t_ray *ray, t_minirt *mrt, t_vector2 pos)
{
	t_camera	*camera;

	camera = mrt->camera;
	ray->origin = camera->position;
	ray->direction = primary_ray(camera, pos, ((float)WIN_WIDTH
				/ (float)WIN_HEIGHT) * camera->iplane_scale);
	ray->dist = 3.4E+38;
	if (mrt->selected)
		mrt->selected->selected = 0;
	mrt->selected = NULL;
	mrt->mlx->update = 1;
	mrt->mlx->count = 0;
}

static inline void	check_object(t_object *object, t_ray *ray,
		float (*intersect)(t_ray *, t_object *, float), t_minirt *mrt)
{
	float		d;

	d = intersect(ray, object, 1.0f);
	if (d == -1.0f || d >= ray->dist)
		return ;
	if (mrt->selected)
		mrt->selected->selected = 0;
	mrt->selected = object;
	mrt->selected->selected = 1;
	ray->dist = d;
}
