/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static int		key_hook(int keycode, t_minirt *mrt);
static int		close_window(t_minirt *mrt);
/* -------------------------------------------------------------------------- */

// pour Linux: mlx_hook(win_ptr, 17, 1L << 0, *f, *mrt);
// pour Linux: mlx_hook(win_ptr, 17, 1L << 17, *f, *mrt);
void	init_mlx(t_mlx *mlx)
{
	t_minirt		*mrt;

	mrt = minirt();
	mlx->mlx_ptr = mlx_init();
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr, WIN_HEIGHT, WIN_WIDTH, "Cetait une blague");
	mlx->img_ptr = mlx_new_image(mlx->mlx_ptr, WIN_HEIGHT, WIN_WIDTH);
	mlx_hook(mlx->win_ptr, 2, 0, key_hook, mrt);
	mlx_hook(mlx->win_ptr, 17, 0, close_window, mrt);
}

static int	key_hook(int keycode, t_minirt *mrt)
{
	if (keycode == MAIN_PAD_ESC)
		close_window(mrt);
	return (0);
}

static int	close_window(t_minirt *mrt)
{
	destruct_minirt(mrt);
	printf("Window closed\n");
	exit(0);
}
