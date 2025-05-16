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
float		intersection(t_ray *ray, t_plane *plane);
void		put_pixel(t_mlx *mlx, t_fvector2 *v, t_rgb *rgb);
/* -------------------------------------------------------------------------- */

void	init_cam_and_plane(void)
{
	t_minirt		*mrt;
	t_fvector3		pos;
	t_fvector3		norm;
	t_rgb			rgb;

	mrt = minirt();
	pos = ft_fvector3(-50.0f, 0.0f, 20.0f);
	norm = ft_fvector3(0.0f, 0.0f, 1.0f);
	mrt->camera = camera(pos, norm, 70);
	pos = ft_fvector3(0.0f, 0.0f, -10.0f);
	norm = ft_fvector3(0.0f, 1.0f, 0.0f);
	rgb = ft_rgb(255, 0, 0);
	mrt->plane = plane(pos, norm, rgb);
}

//ray.direction = ray_tracer();
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
			inter = intersection(&ray, mrt->plane);
			if (inter > 0)
				put_pixel(mrt->mlx, &v, &(t_rgb){255, 0, 0});
			else
				put_pixel(mrt->mlx, &v, &(t_rgb){0, 0, 0});
			v.x++;
		}
		v.y++;
	}
}

void	put_pixel(t_mlx *mlx, t_fvector2 *v, t_rgb *rgb)
{
	if (v->x < 0 || v->x >= WIN_WIDTH || v->y < 0 || v->y >= WIN_HEIGHT)
		return ;
	*((unsigned int *)
			(mlx->data + (int)(v->y * mlx->size_line + v->x * (mlx->bpp / 8))))
		= (rgb->r << 16 | rgb->g << 8 | rgb->b);
}

float	intersection(t_ray *ray, t_plane *plane)
{
	t_fvector3		vec;
	float			denominator;
	float			t;

	denominator = dot(&ray->direction, &plane->normal);
	if (fabs(denominator) < 1e-6)
		return (-1.0f);
	vec = sub_vectors(&plane->position, &ray->origin);
	t = dot(&vec, &plane->normal) / denominator;
	if (t >= 0)
		return (t);
	return (-1.0f);
}

// aspect ratio
// scale
// imagex; imagey;
// direction
//ray.origin = cam->posit;
//ray.direction = normalize();
t_ray	get_ray(t_camera *cam, t_fvector2 uv)
{
	t_ray		ray;

	(void)ray.origin;
	(void)ray.direction;
	(void)uv;
	(void)cam;
	return (ray);
}
