/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/28 17:49:27 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void		*render_part(void *value);
static inline void		init_thread_data(t_thread_data *data, int cores, int i,
							int pixels_per_thread);
/* -------------------------------------------------------------------------- */

void	render_scene(t_minirt *mrt)
{
	int				cores;
	t_thread_data	*datas;
	int				pixels_per_thread;
	int				i;
	t_mlx			*mlx;

	cores = mrt->cores;
	datas = malloc(sizeof(t_thread_data) * cores);
	if (!datas)
		return ;
	pixels_per_thread = (WIN_WIDTH * WIN_HEIGHT) / cores;
	i = -1;
	while (++i < cores)
	{
		datas[i].mrt = mrt;
		init_thread_data(&datas[i], cores, i, pixels_per_thread);
		pthread_create(&datas[i].thread, NULL, render_part, &datas[i]);
	}
	i = -1;
	while (++i < cores)
		pthread_join(datas[i].thread, NULL);
	free(datas);
	mlx = mrt->mlx;
	mlx->count++;
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
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

t_fvector3	primary_ray(t_camera *cam, t_vector2 pos, float ratio)
{
	t_fvector3	ndc_vec;

	ndc_vec = (t_fvector3){
		-(2.0f * (((float)pos.x + 0.5f) / WIN_WIDTH) - 1.0f) * ratio,
		-(2.0f * (((float)pos.y + 0.5f) / WIN_HEIGHT) - 1.0f)
		* cam->iplane_scale, 1.0f};
	return (ft_fnormalize(
			ft_fvector3_sum(
				ft_fvector3_sum(
					(t_fvector3){cam->right.x * ndc_vec.x,
					cam->right.y * ndc_vec.x, cam->right.z * ndc_vec.x},
				(t_fvector3){cam->up.x * ndc_vec.y,
				cam->up.y * ndc_vec.y, cam->up.z * ndc_vec.y}),
		cam->normal)));
}

inline void	init_thread_data(t_thread_data *data, int cores, int i,
		int pixels_per_thread)
{
	data->start = i * pixels_per_thread;
	if (i == cores - 1)
		data->end = WIN_WIDTH * WIN_HEIGHT;
	else
		data->end = (i + 1) * pixels_per_thread;
	data->camera = data->mrt->camera;
	data->ratio = ((float)WIN_WIDTH / (float)WIN_HEIGHT)
		* data->camera->iplane_scale;
}

t_rgb	ray_tracer(t_minirt *mrt, t_ray *ray, int depth)
{
	t_object	*cur;
	void		(*render)(t_minirt *, t_ray *, t_object *, int);

	if (depth > MAX_DEPTH)
		return (ray->color);
	ray->dist = 3.4E+38;
	cur = mrt->objects;
	while (cur)
	{
		render = cur->methods->render;
		if (render)
			render(mrt, ray, cur, depth);
		cur = cur->next;
	}
	if (ray->dist >= 3.4E+37)
		ray->color = (t_rgb){0, 0, 0};
	return (ray->color);
}
