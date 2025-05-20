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
static inline void		intercept(t_minirt *mrt, t_fvector2, t_ray ray);
static inline t_fvector3	ray_tracer(t_camera *cam, t_fvector2 v,
							float ratio);
/* -------------------------------------------------------------------------- */

void	render_scene(t_minirt *mrt)
{
	float		ratio;
	t_mlx		*mlx;
	t_fvector2	v;
	t_ray		ray;

	v.y = 0;
	mlx = mrt->mlx;
	ratio = (float)WIN_WIDTH / (float)WIN_HEIGHT * mrt->camera->iplane_scale;
	ft_bzero(mlx->data, WIN_HEIGHT * mlx->size_line);
	ray.origin = mrt->camera->position;
	while (v.y < WIN_HEIGHT)
	{
		v.x = 0;
		while (v.x < WIN_WIDTH)
		{
			ray.direction = ray_tracer(mrt->camera, v, ratio);
			intercept(mrt, v, ray);
			v.x++;
		}
		v.y++;
	}
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
}

static inline void	intercept(t_minirt *mrt, t_fvector2 v, t_ray ray)
{
	t_object	*cur;
	void		(*render)(t_mlx *, t_ray *, t_fvector2, t_object *);

	ray.dist = 3.4E+38;
	cur = mrt->objects;
	while (cur)
	{
		render = get_render_by_id(cur->id);
		if (render)
			render(mrt->mlx, &ray, v, cur);
		cur = cur->next;
	}
}

static inline t_fvector3	ray_tracer(t_camera *cam, t_fvector2 v, float ratio)
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
