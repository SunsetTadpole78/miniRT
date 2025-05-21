/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void		intercept(t_minirt *mrt, t_vector2, t_ray ray);
static inline t_fvector3	ray_tracer(t_camera *cam, t_vector2 v, float ratio);
static inline void		refresh_buffer(t_minirt *mrt, t_vector2 v);
/* -------------------------------------------------------------------------- */

void	render_scene(t_minirt *mrt)
{
	t_vector2	v;
	t_ray		ray;
	t_camera	*camera;
	t_mlx		*mlx;
	float		ratio;

	v.y = 0;
	mlx = mrt->mlx;
	camera = mrt->camera;
	ratio = ((float)WIN_WIDTH / (float)WIN_HEIGHT) * camera->iplane_scale;
	ray.origin = mrt->camera->position;
	while (v.y < WIN_HEIGHT)
	{
		v.x = 0;
		while (v.x < WIN_WIDTH)
		{
			ray.direction = ray_tracer(camera, v, ratio);
			intercept(mrt, v, ray);
			v.x++;
		}
		v.y++;
	}
	mrt->count++;
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
}

static inline t_fvector3	ray_tracer(t_camera *cam, t_vector2 v, float ratio)
{
	t_fvector3	ndc_vec;

	ndc_vec = (t_fvector3){
		-(2.0f * ((v.x + 0.5f) / WIN_WIDTH) - 1.0f) * ratio,
		-(2.0f * ((v.y + 0.5f) / WIN_HEIGHT) - 1.0f) * cam->iplane_scale, 1.0f};
	return (ft_fnormalize(
			ft_fvector3_sum(
				ft_fvector3_sum(
					(t_fvector3){cam->right.x * ndc_vec.x,
					cam->right.y * ndc_vec.x, cam->right.z * ndc_vec.x},
				(t_fvector3){cam->up.x * ndc_vec.y,
				cam->up.y * ndc_vec.y, cam->up.z * ndc_vec.y}),
		cam->normal)));
}

static inline void	intercept(t_minirt *mrt, t_vector2 v, t_ray ray)
{
	t_object	*cur;
	void		(*render)(t_minirt *, t_ray *, t_vector2, t_object *);

	ray.dist = 3.4E+38;
	cur = mrt->objects;
	while (cur)
	{
		render = cur->render;
		if (render)
			render(mrt, &ray, v, cur);
		cur = cur->next;
	}
	refresh_buffer(mrt, v);
}

static inline void	refresh_buffer(t_minirt *mrt, t_vector2 v)
{
	t_fvector3	sample;
	t_fvector3	avg;
	t_mlx		*mlx;
	int			index;

	mlx = mrt->mlx;
	index = (int)v.y * WIN_WIDTH + (int)v.x;
	sample = pixel_to_fvector3(mlx, v.x, v.y);
	mrt->buffer[index] = ft_fvector3_sum(mrt->buffer[index], sample);
	avg = ft_fvector3_scale(mrt->buffer[index], 1.0f / (mrt->count + 1));
	*((unsigned int *)(mlx->data + (int)(v.y * mlx->ll + v.x * mlx->cl)))
		= fvector3_to_pixel(avg);
}
