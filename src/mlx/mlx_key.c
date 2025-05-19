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
static int	key_hook_next(int keycode, t_minirt *mrt);
/* -------------------------------------------------------------------------- */

int	key_hook(int keycode, t_minirt *mrt)
{
	if (keycode == OGLK_ESC || keycode == XK_ESC)
		close_window(mrt);
	else if (keycode == OGLK_FOVP || keycode == XK_FOVP)
		add_fov(mrt, -1);
	else if (keycode == OGLK_FOVM || keycode == XK_FOVM)
		add_fov(mrt, 1);
	else if (keycode == OGLK_LEFT || keycode == XK_LEFT)
		mrt->camera->position = ft_fvector3_sum(mrt->camera->position,
				ft_fvector3_scale(mrt->camera->right, 1.0f));
	else if (keycode == OGLK_RIGHT || keycode == XK_RIGHT)
		mrt->camera->position = ft_fvector3_sum(mrt->camera->position,
				ft_fvector3_scale(mrt->camera->right, -1.0f));
	else if (keycode == OGLK_ARROW_UP || keycode == XK_ARROW_UP)
		mrt->camera->position.y += 1.0f;
	else if (keycode == OGLK_ARROW_DOWN || keycode == XK_ARROW_DOWN)
		mrt->camera->position.y -= 1.0f;
	else if (keycode == OGLK_FORWARD || keycode == XK_FORWARD)
		mrt->camera->position = ft_fvector3_sum(mrt->camera->position,
				ft_fvector3_scale(mrt->camera->normal, 1.0f));
	else if (keycode == OGLK_BACKWARD || keycode == XK_BACKWARD)
		mrt->camera->position = ft_fvector3_sum(mrt->camera->position,
				ft_fvector3_scale(mrt->camera->normal, -1.0f));
	else
		key_hook_next(keycode, mrt);
	return (0);
}

static int	key_hook_next(int keycode, t_minirt *mrt)
{
	if (keycode == OGLK_ARROW_LEFT || keycode == XK_ARROW_LEFT)
		rotate_camera_y(mrt->camera, 0.1f);
	else if (keycode == OGLK_ARROW_RIGHT || keycode == XK_ARROW_RIGHT)
		rotate_camera_y(mrt->camera, -0.1f);
	return (0);
}
