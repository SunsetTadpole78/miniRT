/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/14 18:32:36 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
			WIN_HEIGHT, WIN_WIDTH, "miniRT");
	mlx->img_ptr = mlx_new_image(mlx->mlx_ptr, WIN_HEIGHT, WIN_WIDTH);
	mlx_hook(mlx->win_ptr, 2, 1L << 0, key_hook, mrt);
	mlx_hook(mlx->win_ptr, 17, 1L << 17, close_window, mrt);
}

static int	key_hook(int keycode, t_minirt *mrt)
{
	if (keycode == MAC_ESC_KEY || keycode == LINUX_ESC_KEY)
		close_window(mrt);
	return (0);
}

static int	close_window(t_minirt *mrt)
{
	destruct_minirt(mrt, 1);
	exit(0);
}
