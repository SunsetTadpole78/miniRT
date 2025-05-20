/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
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
	if (code != 0)
	{
		destruct_minirt(mrt, 0);
		return (2 + code);
	}
	mlx = mrt->mlx;
	update_values(mrt);
	init_mlx(mlx);
	render_scene(mrt);
	handle_events(mrt);
	mlx_loop_hook(mlx->mlx_ptr, loop_hook, mrt);
	mlx_loop(mlx->mlx_ptr);
	return (0);
}
