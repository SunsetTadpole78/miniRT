/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JusteTest.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/17 19:03:48 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
void				render_scene(t_minirt *mrt);
static t_fvector3	ray_tracer(t_camera *cam, t_fvector2 v);
static void			intercept(t_minirt *mrt, t_fvector2, t_ray ray);
/* -------------------------------------------------------------------------- */

void	render_scene(t_minirt *mrt)
{
	t_fvector2		v;
	t_ray			ray;

	v.y = 0;
	ray.origin = mrt->camera->position;
	while (v.y < WIN_HEIGHT)
	{
		v.x = 0;
		while (v.x < WIN_WIDTH)
		{
			ray.direction = ray_tracer(mrt->camera, v);
			intercept(mrt, v, ray);
			v.x++;
		}
		v.y++;
	}
}

static void	intercept(t_minirt *mrt, t_fvector2 v, t_ray ray)
{
	t_object		*cur;
	void			(*render)(t_ray *, t_fvector2, t_object *);

	ray.t = FLT_MAX;
	cur = mrt->objects;
	while (cur)
	{
		render = get_render_by_id(cur->id);
		if (render)
			render(&ray, v, cur);
		cur = cur->next;
	}
}

// normalisation sur [-1, 1]; Normalized Device Coordinates
static t_fvector3	ray_tracer(t_camera *cam, t_fvector2 v)
{
	t_fvector3		ray_dir;
	t_fvector3		ray_world;
	float			norm_x;
	float			norm_y;

	norm_x = (2 * ((v.x + 0.5f) / WIN_WIDTH) - 1)
		* ((float)WIN_WIDTH / (float)WIN_HEIGHT) * cam->iplane_scale;
	norm_y = (1 - 2 * ((v.y + 0.5f) / WIN_HEIGHT)) * cam->iplane_scale;
	ray_dir = normalize(ft_fvector3(norm_x, norm_y, 1.0f));
	ray_world = normalize(mat4_mult_dir(cam->world, ray_dir));
	return (ray_world);
}
