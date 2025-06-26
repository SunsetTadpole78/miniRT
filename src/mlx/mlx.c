/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/02 13:35:53 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

t_mlx	*init_mlx(t_mlx *mlx)
{
	mlx->mlx_ptr = mlx_init();
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr,
			WIN_WIDTH, WIN_HEIGHT, WINDOW_NAME);
	mlx->image.ptr = mlx_new_image(mlx->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	mlx->image.data = mlx_get_data_addr(mlx->image.ptr, &mlx->image.bpp,
			&mlx->image.ll, &mlx->image.endian);
	mlx->image.cl = mlx->image.bpp / 8;
	mlx->image.ratio = (float)WIN_WIDTH / (float)WIN_HEIGHT;
	mlx->update = 0;
	mlx->count = 0;
	return (mlx);
}

void	handle_events(t_minirt *mrt)
{
	t_mlx	*mlx;

	mlx = mrt->mlx;
	mlx_hook(mlx->win_ptr, 2, 1L << 0, on_press_key, mrt);
	mlx_hook(mlx->win_ptr, 3, 1L << 1, on_release_key, mrt);
	mlx_hook(mlx->win_ptr, 4, 1L << 2, on_click, mrt);
	mlx_hook(mlx->win_ptr, 12, 1L << 15, on_expose, mlx);
	mlx_hook(mlx->win_ptr, 17, 1L << 17, close_window, mrt);
}

int	loop_hook(t_minirt *mrt)
{
	t_mlx		*mlx;
	static int	count = 0;

	mlx = mrt->mlx;
	if (mlx->update != 0)
	{
		render_scene(mrt);
		if (mlx->update == 1)
		{
			count = 0;
			mlx->update = 0;
		}
		else
		{
			if (count == 15)
			{
				count = 0;
				mlx->update = 0;
			}
			count++;
		}
	}
	return (0);
}

int	close_window(t_minirt *mrt)
{
	destruct_minirt(mrt, 1);
	exit(0);
}
