/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   press_key.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/02 13:29:31 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline int		refresh_key(t_mlx *mlx);
static inline void	c_key(t_minirt *mrt);
static inline void	paste_object_key(t_minirt *mrt);
static inline void	objects_key(t_minirt *mrt, int keycode);
/* -------------------------------------------------------------------------- */

int	on_press_key(int keycode, t_minirt *mrt)
{
	if (keycode == OGLK_ESC || keycode == XK_ESC)
		close_window(mrt);
	else if (keycode == OGLK_R || keycode == XK_R)
		return (refresh_key(mrt->mlx));
	else if ((keycode == OGLK_C || keycode == XK_C) && mrt->selected)
		c_key(mrt);
	else if (keycode == OGLK_LEFT_COMMAND || keycode == OGLK_RIGHT_COMMAND
		|| keycode == XK_LEFT_CONTROL || keycode == XK_RIGHT_CONTROL)
		mrt->ctrl_pressed++;
	else if ((keycode == OGLK_V || keycode == XK_V) && mrt->ctrl_pressed != 0)
		paste_object_key(mrt);
	else if ((keycode == OGLK_DELETE || keycode == XK_DELETE) && mrt->selected)
		delete_object(mrt, mrt->selected);
	else if (!mrt->selected)
		on_press_key_camera(mrt->camera, keycode);
	else
		objects_key(mrt, keycode);
	mrt->mlx->count = 0;
	mrt->mlx->update = 1;
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

static inline void	c_key(t_minirt *mrt)
{
	if (mrt->ctrl_pressed != 0)
	{
		free(mrt->clipboard);
		mrt->clipboard = mrt->selected->methods->duplicate(
				mrt->selected
				);
	}
	else
	{
		mrt->selected->selected = 0;
		mrt->selected = NULL;
	}
}

static inline void	paste_object_key(t_minirt *mrt)
{
	t_normal_object	*object;

	if (!mrt->clipboard)
		return ;
	object = (t_normal_object *)(mrt->clipboard->methods->duplicate(
				mrt->clipboard
				));
	object->position.x += 1;
	object->position.y += 1;
	object->position.z += 1;
	object->next = mrt->objects;
	object->selected = 1;
	mrt->objects = (t_object *)object;
	if (mrt->selected)
		mrt->selected->selected = 0;
	mrt->selected = mrt->objects;
}

static inline void	objects_key(t_minirt *mrt, int keycode)
{
	void	(*on_press_key)(t_object *, int, t_camera *);

	on_press_key = mrt->selected->methods->on_press_key;
	if (on_press_key)
		on_press_key(mrt->selected, keycode, mrt->camera);
}
