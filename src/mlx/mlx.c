/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/17 20:53:30 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static int		key_hook(int keycode, t_minirt *mrt);
static int		close_window(t_minirt *mrt);
/* -------------------------------------------------------------------------- */

void	init_mlx(t_mlx *mlx)
{
	t_minirt		*mrt;

	mrt = minirt();
	mlx->mlx_ptr = mlx_init();
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr,
			WIN_HEIGHT, WIN_WIDTH, WINDOW_NAME);
	mlx->img_ptr = mlx_new_image(mlx->mlx_ptr, WIN_HEIGHT, WIN_WIDTH);
	mlx_hook(mlx->win_ptr, 2, 1L << 0, key_hook, mrt);
	mlx_hook(mlx->win_ptr, 17, 1L << 17, close_window, mrt);
}

static int	key_hook(int keycode, t_minirt *mrt)
{
	if (keycode == OGLK_ESC || keycode == XK_ESC)
		close_window(mrt);
	else if (keycode == OGLK_FOVP || keycode == XK_FOVP)
		add_fov(mrt, 1);
	else if (keycode == OGLK_FOVM || keycode == XK_FOVM)
		add_fov(mrt, -1);
	else if (keycode == OGLK_LEFT || keycode == XK_LEFT)
		move(mrt, &mrt->camera->position.x, 1.0f);
	else if (keycode == OGLK_RIGHT || keycode == XK_RIGHT)
		move(mrt, &mrt->camera->position.x, -1.0f);
	else if (keycode == OGLK_UP || keycode == XK_UP)
		move(mrt, &mrt->camera->position.y, 1.0f);
	else if (keycode == OGLK_DOWN || keycode == XK_DOWN)
		move(mrt, &mrt->camera->position.y, -1.0f);
	else if (keycode == OGLK_FORWARD || keycode == XK_FORWARD)
		move(mrt, &mrt->camera->position.z, 1.0f);
	else if (keycode == OGLK_BACKWARD || keycode == XK_BACKWARD)
		move(mrt, &mrt->camera->position.z, -1.0f);
	return (0);
}

static int	close_window(t_minirt *mrt)
{
	destruct_minirt(mrt, 1);
	exit(0);
}
