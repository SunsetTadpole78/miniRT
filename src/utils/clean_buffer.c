/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_buffer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	clean_buffer(t_minirt *mrt)
{
	int		i;
	int		scale;
	t_mlx	*mlx;

	i = 0;
	mlx = mrt->mlx;
	scale = WIN_HEIGHT * WIN_WIDTH;
	ft_bzero(mlx->data, WIN_HEIGHT * mlx->ll);
	while (i < scale)
	{
		mrt->buffer[i] = (t_fvector3){0.0f, 0.0f, 0.0f};
		i++;
	}
	mrt->count = 0;
}
