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
	render_scene(mrt);
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
	mlx_loop(mlx->mlx_ptr);
	return (0);
}

void	put_pixel(t_mlx *mlx, t_fvector2 v, t_rgb rgb)
{
	if (v.x < 0 || v.x >= WIN_WIDTH || v.y < 0 || v.y >= WIN_HEIGHT)
		return ;
	*((unsigned int *)
			(mlx->data + (int)(v.y * mlx->size_line + v.x * (mlx->bpp / 8))))
		= (rgb.r << 16 | rgb.g << 8 | rgb.b);
}
