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
static inline t_fvector3	primary_ray(t_camera *cam, t_vector2 pos,
								float ratio);
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
			ray.direction = primary_ray(camera, pos, ratio);
			ray_tracer(mrt, &ray, 0);
			blender(mrt, &ray, pos);
			pos.x++;
		}
		pos.y++;
	}
	mlx->count++;
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
}

static inline t_fvector3	primary_ray(t_camera *cam,
	t_vector2 pos, float ratio)
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
		render = cur->render;
		if (render)
			render(mrt, ray, cur, depth);
		cur = cur->next;
	}
	if (ray->dist >= 3.4E+37)
		ray->color = (t_rgb){0, 0, 0};
	return (ray->color);
}
