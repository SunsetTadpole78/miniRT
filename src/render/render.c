/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/24 19:19:14 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void		*render_part(void *value);
static inline void		init_threads(t_minirt *mrt);
static inline void		fill_image(t_thread_data *data);
static inline void		wait_workers(t_minirt *mrt, int cores);
/* -------------------------------------------------------------------------- */

void	render_scene(t_minirt *mrt)
{
	int				cores;
	t_thread_data	*datas;
	t_mlx			*mlx;
	int				i;

	init_threads(mrt);
	cores = mrt->cores;
	datas = mrt->threads_datas;
	fill_image(&datas[cores - 1]);
	mlx = mrt->mlx;
	mlx->count++;
	wait_workers(mrt, cores);
	i = 0;
	while (i < (cores - 1))
	{
		sem_post(mrt->workers_sem);
		i++;
	}
	usleep(10);
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->image.ptr, 0, 0);
}

static inline void	*render_part(void *value)
{
	t_thread_data	*data;
	int				init;

	data = (t_thread_data *)value;
	init = 1;
	while (!can_exit(data->mrt))
	{
		if (!init)
		{
			pthread_mutex_lock(&data->update_mutex);
			pthread_cond_wait(&data->update_cond, &data->update_mutex);
			pthread_mutex_unlock(&data->update_mutex);
		}
		else
			init = 0;
		if (can_exit(data->mrt))
			return (NULL);
		fill_image(data);
		sem_post(data->mrt->workers_sem);
	}
	return (NULL);
}

static inline void	init_threads(t_minirt *mrt)
{
	int				i;
	int				cores;
	t_thread_data	*datas;
	int				count;

	cores = mrt->cores;
	datas = mrt->threads_datas;
	count = mrt->mlx->count;
	wait_workers(mrt, cores);
	i = 0;
	while (i < (cores - 1))
	{
		if (!mrt->threads_init)
		{
			datas[i].count = count;
			pthread_create(&datas[i].thread, NULL, render_part, &datas[i]);
		}
		else
		{
			pthread_mutex_lock(&datas[i].update_mutex);
			pthread_cond_signal(&datas[i].update_cond);
			pthread_mutex_unlock(&datas[i].update_mutex);
		}
		i++;
	}
	mrt->threads_init = 1;
}

static inline void	fill_image(t_thread_data *data)
{
	t_vector2		pos;
	t_ray			ray;
	int				index;

	ray.origin = data->camera->position;
	index = data->start;
	while (index < data->end)
	{
		pos.x = index % WIN_WIDTH;
		pos.y = index / WIN_WIDTH;
		ray.direction = primary_ray(data->camera, pos, data->ratio);
		ray_tracer(data->mrt, &ray, 0);
		blend_colors(data->mrt, &ray, pos, data->count);
		index++;
	}
}

static inline void	wait_workers(t_minirt *mrt, int cores)
{
	int	i;

	i = 0;
	while (i < (cores - 1))
	{
		sem_wait(mrt->workers_sem);
		i++;
	}
}
