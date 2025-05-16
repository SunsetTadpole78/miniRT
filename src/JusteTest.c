/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
void		init_cam_and_plane(void);
void		render_scene(t_minirt *mrt);
float		intersection(t_ray ray, t_plane *plane);
t_fvector3	ray_tracer(t_camera *cam, t_fvector2 v);
void		put_pixel(t_mlx *mlx, t_fvector2 v, t_rgb rgb);
/* -------------------------------------------------------------------------- */

// la nouvelle fov permet d'etre adapte a cette normalisation.
void	init_cam_and_plane(void)
{
	t_minirt		*mrt;
	t_fvector3		pos;
	t_fvector3		norm;
	t_rgb			rgb;

	mrt = minirt();
	pos = ft_fvector3(-50.0f, 10.0f, 20.0f);
	norm = ft_fvector3(0.0f, 0.0f, 1.0f);
	mrt->camera = camera(pos, norm, 70);
	mrt->camera->norm_fov = tan((mrt->camera->fov / 2) * (M_PI / 180.0f));
	pos = ft_fvector3(0.0f, 0.0f, -10.0f);
	norm = ft_fvector3(0.0f, 1.0f, 0.0f);
	rgb = ft_rgb(255, 0, 0);
	mrt->plane = plane(pos, norm, rgb);
}

void	render_scene(t_minirt *mrt)
{
	t_fvector2		v;
	t_ray			ray;
	float			inter;

	v.y = 0;
	ray.origin = mrt->camera->position;
	while (v.y < WIN_HEIGHT)
	{
		v.x = 0;
		while (v.x < WIN_WIDTH)
		{
			ray.direction = ray_tracer(mrt->camera, v);
			inter = intersection(ray, mrt->plane);
			if (inter > 0)
				put_pixel(mrt->mlx, v, (t_rgb){255, 0, 0});
			else
				put_pixel(mrt->mlx, v, (t_rgb){0, 0, 0});
			v.x++;
		}
		v.y++;
	}
}

void	put_pixel(t_mlx *mlx, t_fvector2 v, t_rgb rgb)
{
	if (v.x < 0 || v.x >= WIN_WIDTH || v.y < 0 || v.y >= WIN_HEIGHT)
		return ;
	*((unsigned int *)
			(mlx->data + (int)(v.y * mlx->size_line + v.x * (mlx->bpp / 8))))
		= (rgb.r << 16 | rgb.g << 8 | rgb.b);
}

// t = ((P - O) * N) / D * N
// P = point du plan.		O = origin du rayon.
// D = direction du rayon.	N = normal du plan.
float	intersection(t_ray ray, t_plane *plane)
{
	t_fvector3		vec;
	float			denominator;
	float			t;

	denominator = dot(ray.direction, plane->normal);
	if (fabs(denominator) < 1e-6)
		return (-1.0f);
	vec = sub_vectors(plane->position, ray.origin);
	t = dot(vec, plane->normal) / denominator;
	if (t >= 0)
		return (t);
	return (-1.0f);
}

// normalisation sur [-1, 1];
t_fvector3	ray_tracer(t_camera *cam, t_fvector2 v)
{
	t_fvector3		ray_dir;
	float			norm_x;
	float			norm_y;

	norm_x = (2 * ((v.x + 0.5f) / WIN_WIDTH) - 1) * RATIO * cam->norm_fov;
	norm_y = (1 - 2 * ((v.y + 0.5f) / WIN_HEIGHT)) * cam->norm_fov;
	ray_dir = normalize(ft_fvector3(norm_x, norm_y, -1.0f));
	return (ray_dir);
}
