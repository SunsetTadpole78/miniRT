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

void	init_mlx(t_mlx *mlx)
{
	mlx->mlx_ptr = mlx_init();
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr,
			WIN_WIDTH, WIN_HEIGHT, WINDOW_NAME);
	mlx->img_ptr = mlx_new_image(mlx->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	mlx->data = mlx_get_data_addr(mlx->img_ptr, &mlx->bpp,
			&mlx->size_line, &mlx->endian);
}

int	key_hook(int keycode, t_minirt *mrt)
{
	if (keycode == OGLK_ESC || keycode == XK_ESC)
		close_window(mrt);
	else if (keycode == OGLK_FOVP || keycode == XK_FOVP)
		add_fov(mrt, -1);
	else if (keycode == OGLK_FOVM || keycode == XK_FOVM)
		add_fov(mrt, 1);
	else if (keycode == OGLK_LEFT || keycode == XK_LEFT)
		mrt->camera->position.x -= 1.0f;
	else if (keycode == OGLK_RIGHT || keycode == XK_RIGHT)
		mrt->camera->position.x += 1.0f;
	else if (keycode == OGLK_ARROW_UP || keycode == XK_ARROW_UP)
		mrt->camera->position.y += 1.0f;
	else if (keycode == OGLK_ARROW_DOWN || keycode == XK_ARROW_DOWN)
		mrt->camera->position.y -= 1.0f;
	else if (keycode == OGLK_FORWARD || keycode == XK_FORWARD)
		mrt->camera->position.z += 1.0f;
	else if (keycode == OGLK_BACKWARD || keycode == XK_BACKWARD)
		mrt->camera->position.z -= 1.0f;
	else if (keycode == OGLK_ARROW_LEFT || keycode == XK_ARROW_LEFT)
		rotate_camera_y(mrt->camera, 0.1f);
	else if (keycode == OGLK_ARROW_RIGHT || keycode == XK_ARROW_RIGHT)
		rotate_camera_y(mrt->camera, -0.1f);
	return (0);
}

void	handle_events(t_minirt *mrt)
{
	t_mlx		*mlx;

	mlx = mrt->mlx;
	mlx_hook(mlx->win_ptr, 2, 1L << 0, key_hook, mrt);
	mlx_hook(mlx->win_ptr, 17, 1L << 17, close_window, mrt);
}

int	loop_hook(t_minirt *mrt)
{
	render_scene(mrt);
	return (0);
}

int	close_window(t_minirt *mrt)
{
	destruct_minirt(mrt, 1);
	exit(0);
}
