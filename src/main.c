/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:31:29 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/14 18:16:55 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

int	main(int argc, char **argv)
{
	t_minirt	*mrt;
	int			code;

	if (argc != 2)
		return (ft_error(USAGE_ERROR, RED, 1));
	mrt = minirt();
	if (!mrt)
		return (ft_error(MALLOC_ERROR, RED, 2));
	code = parse_map(argv[1]);
	if (code != 0)
		code += 2;
	init_mlx(mrt->mlx);
	mlx_loop(mrt->mlx->mlx_ptr);
	return (0);
}
