/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/20 19:02:05 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static t_fvector3	ray_tracer(t_camera *cam, t_vector2 position, float ratio);
static void			intercept(t_minirt *mrt, t_vector2 position, t_ray ray);
/* -------------------------------------------------------------------------- */

void	render_scene(t_minirt *mrt)
{
	t_vector2		position;
	t_ray			ray;
	t_camera	*camera;
	float		ratio;

	position.y = 0;
	ray.origin = mrt->camera->position;
	camera = mrt->camera;
	ratio = ((float)WIN_WIDTH / (float)WIN_HEIGHT) * camera->iplane_scale;
	while (position.y < WIN_HEIGHT)
	{
		position.x = 0;
		while (position.x < WIN_WIDTH)
		{
			ray.direction = ray_tracer(camera, position, ratio);
			intercept(mrt, position, ray);
			position.x++;
		}
		position.y++;
	}
}

static void	intercept(t_minirt *mrt, t_vector2 position, t_ray ray)
{
	t_object	*cur;
	void		(*render)(t_minirt *, t_ray *, t_object *);
	t_mlx	*mlx;

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
	cur = (t_object *)mrt->lights;
	while (cur)
	{
	//	render_light(mrt, &ray, cur);
		cur = cur->next;
	}
	*((unsigned int *)(mlx->data + (position.y * mlx->ll + position.x * mlx->cl)))
		= (ray.color.r << 16 | ray.color.g << 8 | ray.color.b);
}

// normalisation sur [-1, 1]; Normalized Device Coordinates
static t_fvector3	ray_tracer(t_camera *cam, t_vector2 position, float ratio)
{
	return (ft_fnormalize(ft_fvector3(
				(2.0f * (((float)position.x + 0.5f) / WIN_WIDTH) - 1.0f) * ratio,
				(1.0f - 2.0f * (((float)position.y + 0.5f) / WIN_HEIGHT))
				* cam->iplane_scale,
				1.0f
			)));
}
