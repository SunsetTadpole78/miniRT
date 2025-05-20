/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/20 15:03:58 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static t_fvector3	ray_tracer(t_camera *cam, t_fvector2 v);
static void			intercept(t_minirt *mrt, t_fvector2 position,
						t_ray ray);
/* -------------------------------------------------------------------------- */

void	render_scene(t_minirt *mrt)
{
	t_fvector2		position;
	t_ray			ray;

	position.y = 0;
	ray.origin = mrt->camera->position;
	while (position.y < WIN_HEIGHT)
	{
		position.x = 0;
		while (position.x < WIN_WIDTH)
		{
			ray.direction = ray_tracer(mrt->camera, position);
			intercept(mrt, position, ray);
			position.x++;
		}
		position.y++;
	}
}

static void	intercept(t_minirt *mrt, t_fvector2 position, t_ray ray)
{
	t_object		*cur;
	void			(*render)(t_minirt *, t_ray *, t_object *);

	ray.dist = 3.4E+38;
	ray.color = ft_rgb(0, 0, 0);
	cur = mrt->objects;
	while (cur)
	{
		render = get_render_by_id(cur->id);
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
	put_pixel(mrt->mlx, position, ray.color);
}

// normalisation sur [-1, 1]; Normalized Device Coordinates
static t_fvector3	ray_tracer(t_camera *cam, t_fvector2 v)
{
	return (ft_fnormalize(
			ft_fvector3((2 * ((v.x + 0.5f) / WIN_WIDTH) - 1)
				* ((float)WIN_WIDTH / (float)WIN_HEIGHT)
				* cam->iplane_scale,
				(1 - 2 * ((v.y + 0.5f) / WIN_HEIGHT))
				* cam->iplane_scale, 1.0f)));
}

void	put_pixel(t_mlx *mlx, t_fvector2 v, t_rgb rgb)
{
	if (v.x < 0 || v.x >= WIN_WIDTH || v.y < 0 || v.y >= WIN_HEIGHT)
		return ;
	*((unsigned int *)
			(mlx->data + (int)(v.y * mlx->size_line + v.x * (mlx->bpp / 8))))
		= (rgb.r << 16 | rgb.g << 8 | rgb.b);
}
