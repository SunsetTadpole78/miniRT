/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:31:29 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/27 21:24:17 by lroussel         ###   ########.fr       */
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
	code = parse_map(argv[1]);
	if (code != 0 || !check_env(mrt))
	{
		destruct_minirt(mrt, 0);
		return (4 + code);
	}
	mlx = init_mlx(mrt->mlx);
	render_scene(mrt);
	handle_events(mrt);
	mlx_loop_hook(mlx->mlx_ptr, loop_hook, mrt);
	mlx_loop(mlx->mlx_ptr);
	return (0);
}
