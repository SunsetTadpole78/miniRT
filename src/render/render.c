/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/20 01:15:56 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static t_fvector3	ray_tracer(t_camera *cam, t_vector2 v, float ratio);
static void			intercept(t_minirt *mrt, t_vector2, t_ray ray);
/* -------------------------------------------------------------------------- */

void	render_scene(t_minirt *mrt)
{
	t_vector2	v;
	t_ray		ray;
	t_camera	*camera;
	float		ratio;

	v.y = 0;
	ray.origin = mrt->camera->position;
	camera = mrt->camera;
	ratio = ((float)WIN_WIDTH / (float)WIN_HEIGHT) * camera->iplane_scale;
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
}

static void	intercept(t_minirt *mrt, t_vector2 v, t_ray ray)
{
	t_object		*cur;
	void			(*render)(t_minirt *, t_ray *, t_vector2, t_object *);

	ray.dist = 3.4E+38;
	cur = mrt->objects;
	while (cur)
	{
		render = cur->render;
		if (render)
			render(mrt, &ray, v, cur);
		cur = cur->next;
	}
}

// normalisation sur [-1, 1]; Normalized Device Coordinates
static t_fvector3	ray_tracer(t_camera *cam, t_vector2 v, float ratio)
{
	return (ft_fnormalize(ft_fvector3(
				(2.0f * ((v.x + 0.5f) / WIN_WIDTH) - 1.0f) * ratio,
				(1.0f - 2.0f * ((v.y + 0.5f) / WIN_HEIGHT)) * cam->iplane_scale,
				1.0f
			)));
}

void	put_pixel(t_mlx *mlx, t_vector2 v, t_rgb rgb)
{
	*((unsigned int *)(mlx->data + (v.y * mlx->ll + v.x * mlx->cl)))
		= (rgb.r << 16 | rgb.g << 8 | rgb.b);
}
