/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/28 21:22:00 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

t_mlx	*init_mlx(t_mlx *mlx)
{
	mlx->mlx_ptr = mlx_init();
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr,
			WIN_WIDTH, WIN_HEIGHT, WINDOW_NAME);
	mlx->img_ptr = mlx_new_image(mlx->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	mlx->data = mlx_get_data_addr(mlx->img_ptr, &mlx->bpp,
			&mlx->ll, &mlx->endian);
	mlx->cl = mlx->bpp / 8;
	mlx->update = 0;
	mlx->count = 0;
	return (mlx);
}

void	handle_events(t_minirt *mrt)
{
	t_mlx	*mlx;

	mlx = mrt->mlx;
	mlx_hook(mlx->win_ptr, 2, 1L << 0, key_hook, mrt);
	mlx_hook(mlx->win_ptr, 17, 1L << 17, close_window, mrt);
	mlx_hook(mlx->win_ptr, 12, 1L << 15, on_expose, mlx);
}

int	loop_hook(t_minirt *mrt)
{
	if (mrt->mlx->update != 0)
	{
		render_scene(mrt);
		if (mrt->mlx->update != 2)
			mrt->mlx->update = 0;
	}
	return (0);
}

int	close_window(t_minirt *mrt)
{
	destruct_minirt(mrt, 1);
	exit(0);
}
