/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x11.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 13:51:40 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/16 13:54:43 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	destruct_mlx(t_mlx *mlx)
{
	mlx_destroy_image(mlx->mlx_ptr, mlx->img_ptr);
	mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr);
	mlx_destroy_display(mlx->mlx_ptr);
	free(mlx->mlx_ptr);
}
