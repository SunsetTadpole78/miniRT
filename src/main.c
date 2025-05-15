/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:31:29 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/13 17:22:56 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	main(int argc, char **argv)
{
	t_minirt	*mrt;
	t_mlx		*mlx;

	(void)argc;
	(void)argv;
	mrt = minirt();
	if (!mrt)
	{
		write(2, "miniRT: Malloc Error.\n", 22);
		return (1);
	}
	mlx = mrt->mlx;
	init_mlx(mlx);
	init_cam_and_plane();
	render_scene(mrt);
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
	mlx_loop(mlx->mlx_ptr);
	return (0);
}
