/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/28 00:35:37 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void		*render_part(void *value);
static inline void		init_threads(t_minirt *mrt);
static inline void		fill_image(t_thread_data *data);
static inline void		blend_colors(unsigned int *ptr, t_rgb color, int count,
							float count_ratio);
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
	mlx = mrt->mlx;
	datas[cores - 1].count = mlx->count;
	datas[cores - 1].count_ratio = 1.0f / (mlx->count + 1);
	fill_image(&datas[cores - 1]);
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
	t_thread_data	*datas;
	int				count;
	float			ratio;

	datas = mrt->threads_datas;
	count = mrt->mlx->count;
	wait_workers(mrt, mrt->cores);
	i = 0;
	ratio = 1.0f / (count + 1);
	while (i < (mrt->cores - 1))
	{
		datas[i].count = count;
		datas[i].count_ratio = ratio;
		if (!mrt->threads_init)
			pthread_create(&datas[i].thread, NULL, render_part, &datas[i]);
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
	int				id;
	unsigned int	*ptr;
	t_mlx_image		image;

	ray.origin = data->camera->position;
	pos.y = 0;
	id = data->id;
	image = data->mrt->mlx->image;
	while (pos.y < WIN_HEIGHT)
	{
		ptr = (unsigned int *)(image.data + (pos.y * image.ll + id * image.cl));
		pos.x = id;
		while (pos.x < WIN_WIDTH)
		{
			ray.direction = primary_ray(data->camera, pos, data->ratio);
			ray_tracer(data->mrt, &ray, 0);
			blend_colors(ptr, ray.color, data->count, data->count_ratio);
			ptr += data->cores;
			pos.x += data->cores;
		}
		pos.y++;
	}
}

static inline void	blend_colors(unsigned int *ptr, t_rgb color, int count,
		float count_ratio)
{
	int	v;

	v = *ptr;
	*ptr = (((unsigned char)((((v >> 16) & 0xFF) * count + color.r)
					* count_ratio)) & 0xFF) << 16
		| ((unsigned char)((((v >> 8) & 0xFF) * count + color.g)
				* count_ratio) & 0xFF) << 8
		| ((unsigned char)(((v & 0xFF) * count + color.b)
				* count_ratio) & 0xFF);
}
