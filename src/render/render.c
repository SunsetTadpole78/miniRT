/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/22 18:42:38 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_fvector3	ray_tracer(t_camera *cam, t_vector2 pos,
								float ratio);
static inline void		intercept(t_minirt *mrt, t_vector2 pos, t_ray ray);
/* -------------------------------------------------------------------------- */

void	render_scene(t_minirt *mrt)
{
	t_vector2	pos;
	t_ray		ray;
	t_camera	*camera;
	t_mlx		*mlx;
	float		ratio;

	pos.y = 0;
	mlx = mrt->mlx;
	camera = mrt->camera;
	ratio = ((float)WIN_WIDTH / (float)WIN_HEIGHT) * camera->iplane_scale;
	ray.origin = mrt->camera->position;
	while (pos.y < WIN_HEIGHT)
	{
		pos.x = 0;
		while (pos.x < WIN_WIDTH)
		{
			ray.direction = ray_tracer(camera, pos, ratio);
			intercept(mrt, pos, ray);
			pos.x++;
		}
		pos.y++;
	}
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
}

static inline void	intercept(t_minirt *mrt, t_vector2 pos, t_ray ray)
{
	t_object	*cur;
	void		(*render)(t_minirt *, t_ray *, t_object *);
	t_mlx		*mlx;

	ray.dist = 3.4E+38;
	ray.color = ft_rgb(0, 0, 0);
	cur = mrt->objects;
	mlx = mrt->mlx;
	while (cur)
	{
		render = cur->render;
		if (render)
			render(mrt, &ray, cur);
		cur = cur->next;
	}
	*((unsigned int *)(mlx->data + (pos.y * mlx->ll + pos.x * mlx->cl)))
		= (ray.color.r << 16 | ray.color.g << 8 | ray.color.b);
}

static inline t_fvector3	ray_tracer(t_camera *cam, t_vector2 pos,
				float ratio)
{
	t_fvector3	ndc_vec;

	ndc_vec = (t_fvector3){
		-(2.0f * (((float)pos.x + 0.5f) / WIN_WIDTH) - 1.0f) * ratio,
		-(2.0f * (((float)pos.y + 0.5f) / WIN_HEIGHT) - 1.0f)
		* cam->iplane_scale,
		1.0f};
	return (ft_fnormalize(
			ft_fvector3_sum(
				ft_fvector3_sum(
					(t_fvector3){cam->right.x * ndc_vec.x,
					cam->right.y * ndc_vec.x, cam->right.z * ndc_vec.x},
				(t_fvector3){cam->up.x * ndc_vec.y,
				cam->up.y * ndc_vec.y, cam->up.z * ndc_vec.y}),
		cam->normal)));
}
