/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/02 13:38:03 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void		*render_part(void *value);
/* -------------------------------------------------------------------------- */

void	render_scene(t_minirt *mrt)
{
	int				cores;
	t_thread_data	*datas;
	int				i;
	t_mlx			*mlx;

	cores = mrt->cores;
	datas = mrt->threads_datas;
	i = 0;
	while (i < (cores - 1))
	{
		pthread_create(&datas[i].thread, NULL, render_part, &datas[i]);
		i++;
	}
	render_part(&datas[i]);
	i = 0;
	while (i < (cores - 1))
	{
		pthread_join(datas[i].thread, NULL);
		i++;
	}
	mlx = mrt->mlx;
	mlx->count++;
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->image.ptr, 0, 0);
}

static inline void	*render_part(void *value)
{
	t_thread_data	*data;
	t_vector2		pos;
	t_ray			ray;
	int				index;

	data = (t_thread_data *)value;
	ray.origin = data->camera->position;
	index = data->start;
	while (index < data->end)
	{
		pos.x = index % WIN_WIDTH;
		pos.y = index / WIN_WIDTH;
		ray.direction = primary_ray(data->camera, pos, data->ratio);
		ray_tracer(data->mrt, &ray, 0);
		blend_colors(data->mrt, &ray, pos);
		index++;
	}
	return (NULL);
}
