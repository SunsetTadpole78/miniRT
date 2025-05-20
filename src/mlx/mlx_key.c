/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_key.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static int	rotation_key_hook(int keycode, t_minirt *mrt);
/* -------------------------------------------------------------------------- */

int	key_hook(int keycode, t_minirt *mrt)
{
	if (keycode == OGLK_FOVP || keycode == XK_FOVP)
		update_fov(mrt, -1);
	else if (keycode == OGLK_FOVM || keycode == XK_FOVM)
		update_fov(mrt, 1);
	else if (keycode == OGLK_LEFT || keycode == XK_LEFT)
		mrt->camera->position = ft_fvector3_sum(mrt->camera->position,
				mrt->camera->right);
	else if (keycode == OGLK_RIGHT || keycode == XK_RIGHT)
		mrt->camera->position = ft_fvector3_sum(mrt->camera->position,
				ft_fvector3_scale(mrt->camera->right, -1.0f));
	else if (keycode == OGLK_SPACE || keycode == XK_SPACE)
		mrt->camera->position.y += 1.0f;
	else if (keycode == OGLK_SHIFT || keycode == XK_SHIFT)
		mrt->camera->position.y -= 1.0f;
	else if (keycode == OGLK_FORWARD || keycode == XK_FORWARD)
		mrt->camera->position = ft_fvector3_sum(mrt->camera->position,
				mrt->camera->normal);
	else if (keycode == OGLK_BACKWARD || keycode == XK_BACKWARD)
		mrt->camera->position = ft_fvector3_sum(mrt->camera->position,
				ft_fvector3_scale(mrt->camera->normal, -1.0f));
	else
		rotation_key_hook(keycode, mrt);
	mrt->mlx->update = 1;
	return (0);
}

static int	rotation_key_hook(int keycode, t_minirt *mrt)
{
	if (keycode == OGLK_ARROW_LEFT || keycode == XK_ARROW_LEFT)
		update_yaw(mrt->camera, 0.1f);
	else if (keycode == OGLK_ARROW_RIGHT || keycode == XK_ARROW_RIGHT)
		update_yaw(mrt->camera, -0.1f);
	else if (keycode == OGLK_ARROW_UP || keycode == XK_ARROW_UP)
		update_pitch(mrt->camera, 0.1f);
	else if (keycode == OGLK_ARROW_DOWN || keycode == XK_ARROW_DOWN)
		update_pitch(mrt->camera, -0.1f);
	else if (keycode == OGLK_ESC || keycode == XK_ESC)
		close_window(mrt);
	return (0);
}

int	release_key_hook(int keycode, t_minirt *mrt)
{
	(void)keycode;
	mrt->mlx->update = 0;
	return (0);
}
