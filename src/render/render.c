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
static t_fvector3	ray_tracer(t_camera *cam, t_fvector2 v);
static void			intercept(t_minirt *mrt, t_fvector2, t_ray ray);
t_fvector3			ft_fvector3_scale(t_fvector3 v, float s);
/* -------------------------------------------------------------------------- */

void	render_scene(t_minirt *mrt)
{
	t_mlx			*mlx;
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
	mlx = mrt->mlx;
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
}

static void	intercept(t_minirt *mrt, t_fvector2 v, t_ray ray)
{
	t_object		*cur;
	void			(*render)(t_mlx *, t_ray *, t_fvector2, t_object *);

	ray.dist = 3.4E+38;
	cur = mrt->objects;
	put_pixel(mrt->mlx, v, (t_rgb){0, 0, 0});
	while (cur)
	{
		render = get_render_by_id(cur->id);
		if (render)
			render(mrt->mlx, &ray, v, cur);
		cur = cur->next;
	}
}

static t_fvector3	ray_tracer(t_camera *cam, t_fvector2 v)
{
	t_fvector3		ndc_vec;

	ndc_vec = (t_fvector3){
		-(2.0f * ((v.x + 0.5f) / WIN_WIDTH) - 1.0f)
		* cam->wid_height * cam->iplane_scale,
		-(2.0f * ((v.y + 0.5f) / WIN_HEIGHT) - 1.0f)
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

void	put_pixel(t_mlx *mlx, t_fvector2 v, t_rgb rgb)
{
	if (v.x < 0 || v.x >= WIN_WIDTH || v.y < 0 || v.y >= WIN_HEIGHT)
		return ;
	*((unsigned int *)
			(mlx->data + (int)(v.y * mlx->size_line + v.x * (mlx->bpp / 8))))
		= (rgb.r << 16 | rgb.g << 8 | rgb.b);
}
