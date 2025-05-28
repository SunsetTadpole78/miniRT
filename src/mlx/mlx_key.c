/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_key.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/28 15:46:34 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void	init_ray(t_ray *ray, t_minirt *mrt, t_vector2 pos);
/* -------------------------------------------------------------------------- */

int	on_press_key(int keycode, t_minirt *mrt)
{
	if (keycode == OGLK_ESC || keycode == XK_ESC)
		close_window(mrt);
	else if (!mrt->selected)
		update_camera(mrt->camera, keycode);
	else
		printf("objs\n");
	mrt->mlx->count = 0;
	mrt->mlx->update = 1;
	return (0);
}

int	on_click(int id, int x, int y, t_minirt *mrt)
{
	t_object	*cur;
	t_ray		ray;
	float		(*intersect)(t_ray, t_object *);
	float		d;

	if (id != 1)
		return (0);
	cur = mrt->objects;
	init_ray(&ray, mrt, (t_vector2){x, y});
	while (cur)
	{
		intersect = cur->intersect;
		if (intersect)
		{
			d = intersect(ray, cur);
			if (d != -1.0f && d < ray.dist)
			{
				mrt->selected = cur;
				ray.dist = d;
			}
		}
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
	mrt->selected = NULL;
}
