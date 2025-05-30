/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_key.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/30 14:36:19 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void	init_ray(t_ray *ray, t_minirt *mrt, t_vector2 pos);
static inline int		refresh_key(t_mlx *mlx);
/* -------------------------------------------------------------------------- */

int	on_press_key(int keycode, t_minirt *mrt)
{
	void	(*on_press_key)(t_object *, int, t_camera *);

	if (keycode == OGLK_ESC || keycode == XK_ESC)
		close_window(mrt);
	else if (keycode == OGLK_R || keycode == XK_R)
		return (refresh_key(mrt->mlx));
	else if ((keycode == OGLK_C || keycode == XK_C) && mrt->selected)
	{
		mrt->selected->selected = 0;
		mrt->selected = NULL;
	}
	else if (!mrt->selected)
		on_press_key_camera(mrt->camera, keycode);
	else
	{
		on_press_key = mrt->selected->methods->on_press_key;
		if (on_press_key)
			on_press_key(mrt->selected, keycode, mrt->camera);
	}
	mrt->mlx->count = 0;
	mrt->mlx->update = 1;
	return (0);
}

int	on_click(int id, int x, int y, t_minirt *mrt)
{
	t_object	*cur;
	t_ray		ray;
	float		(*intersect)(t_ray *, t_object *, float);
	float		d;

	if (id != 1)
		return (0);
	cur = mrt->objects;
	init_ray(&ray, mrt, (t_vector2){x, y});
	while (cur)
	{
		intersect = cur->methods->intersect;
		if (intersect)
		{
			d = intersect(&ray, cur, 1.0f);
			if (d != -1.0f && d < ray.dist)
			{
				mrt->selected = cur;
				mrt->selected->selected = 1;
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
	if (mrt->selected)
		mrt->selected->selected = 0;
	mrt->selected = NULL;
	mrt->mlx->update = 1;
	mrt->mlx->count = 0;
}

int	on_expose(t_mlx *mlx)
{
	mlx->update = 1;
	return (0);
}

static inline int	refresh_key(t_mlx *mlx)
{
	if (mlx->update == 2)
		mlx->update = 1;
	else
		mlx->update = 2;
	return (0);
}
