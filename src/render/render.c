/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/03 08:23:09 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void	*render_part(void *value);
static inline void	init_threads(t_minirt *mrt);
static inline void	fill_image(t_thread_data *data, t_ray *ray,
						t_minirt *mrt);
/* -------------------------------------------------------------------------- */

void	render_scene(t_minirt *mrt)
{
	t_thread_data	*data;
	t_ray			ray;
	t_mlx			*mlx;

	mrt->workers = mrt->cores;
	init_threads(mrt);
	data = &mrt->threads_datas[mrt->cores - 1];
	fill_image(data, &ray, mrt);
	while (is_working(mrt))
	{
		if (is_stop(mrt))
			return ;
		usleep(1000);
		continue ;
	}
	mlx = mrt->mlx;
	mlx->count++;
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->image.ptr, 0, 0);
}

static inline void	*render_part(void *value)
{
	t_thread_data	*data;
	t_ray			ray;
	t_minirt		*mrt;

	data = (t_thread_data *)value;
	mrt = data->mrt;
	while (1)
	{
		while (!is_update(mrt, data))
		{
			if (is_stop(mrt))
				return (NULL);
			usleep(1000);
			continue ;
		}
		fill_image(data, &ray, mrt);
	}
	return (NULL);
}

static inline void	init_threads(t_minirt *mrt)
{
	int				cores;
	t_thread_data	*datas;
	int				i;

	cores = mrt->cores;
	datas = mrt->threads_datas;
	i = 0;
	while (i < (cores - 1))
	{
		if (!datas[i].init)
		{
			pthread_create(&datas[i].thread, NULL, render_part, &datas[i]);
			datas[i].init = 1;
		}
		pthread_mutex_lock(&mrt->update_mutex);
		datas[i].update = 1;
		pthread_mutex_unlock(&mrt->update_mutex);
		i++;
	}
}

static inline void	fill_image(t_thread_data *data, t_ray *ray, t_minirt *mrt)
{
	t_vector2		pos;

	ray->origin = data->camera->position;
	pos.y = data->start;
	while (pos.y < data->end)
	{
		pos.x = 0;
		while (pos.x < WIN_WIDTH)
		{
			ray->direction = primary_ray(data->camera, pos, data->ratio);
			ray_tracer(mrt, ray, 0);
			blend_colors(mrt, ray, pos);
			pos.x++;
		}
		pos.y++;
	}
	data->update = 0;
	on_finish(mrt);
}
