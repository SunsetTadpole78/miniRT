/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:31:29 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/27 17:57:41 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

int	main(int argc, char **argv)
{
	int			code;
	t_minirt	*mrt;
	t_mlx		*mlx;

	if (argc != 2)
		return (ft_error(USAGE_E, ERR_PREFIX, 1));
	mrt = minirt();
	if (!mrt)
		return (ft_error(MALLOC_E, ERR_PREFIX, 2));
	if (!init_cores(mrt))
		return (ft_error(CORES_E, ERR_PREFIX, 3));
	code = parse_map(argv[1]);
	if (code != 0)
	{
		destruct_minirt(mrt, 0);
		return (3 + code);
	}
	if (!mrt->ambiant)
		mrt->ambiant = ambiant(0, (t_rgb){0, 0, 0});
	mlx = mrt->mlx;
	init_mlx(mlx);
	render_scene(mrt);
	handle_events(mrt);
	mlx_loop_hook(mlx->mlx_ptr, loop_hook, mrt);
	mlx_loop(mlx->mlx_ptr);
	return (0);
}
