/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:31:04 by lroussel          #+#    #+#             */
/*   Updated: 2025/04/23 15:36:31 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	destruct_minirt(t_minirt *mrt)
{
	t_mlx		*mlx;

	mlx = mrt->mlx;
	ft_lstclear(&mrt->objects, free);
	ft_lstclear(&mrt->lights, free);
	free(mrt->ambiant);
	free(mrt->camera);
	mlx_destroy_image(mlx->mlx_ptr, mlx->img_ptr);
	mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr);
	free(mlx->mlx_ptr);
	free(mlx);
	free(mrt);
}
