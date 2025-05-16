/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:31:29 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/16 01:14:40 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

int	main(int argc, char **argv)
{
	t_minirt	*mrt;
	int			code;
	t_mlx		*mlx;

	if (argc != 2)
		return (ft_error(USAGE_E, ERR_PREFIX, 1));
	mrt = minirt();
	if (!mrt)
		return (ft_error(MALLOC_E, ERR_PREFIX, 2));
	code = parse_map(argv[1]);
	if (code != 0)
	{
		destruct_minirt(mrt, 0);
		return (2 + code);
	}
	mlx = mrt->mlx;
	init_mlx(mlx);
	init_cam_and_plane();
	render_scene(mrt);
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
	mlx_loop(mlx->mlx_ptr);
	return (0);
}
