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
void				render_scene(t_minirt *mrt);
static t_fvector3	ray_tracer(t_camera *cam, t_fvector2 v);
static void			intercept(t_minirt *mrt, t_fvector2, t_ray ray);
static void			put_pixel(t_mlx *mlx, t_fvector2 v, t_rgb rgb);
static t_rgb		get_color(t_object *object);
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
	float			(*render)(t_ray, t_object *);
	float			t;
	float			prev_t;

	(void)prev_t;
	prev_t = -1;
	cur = mrt->objects;
	while (cur)
	{
		render = get_render_by_id(cur->id);
		if (render)
		{
			t = render(ray, cur);
			if (t > 0)
				put_pixel(mrt->mlx, v, get_color(cur));
			prev_t = t;
		}
		cur = cur->next;
	}
}

static t_rgb	get_color(t_object *object)
{
	char		*id;
	int			len;

	id = ((t_object *)object)->id;
	len = ft_strlen(id) + 1;
	if (ft_strncmp(SPHERE_ID, id, len) == 0)
		return (((t_sphere *)object)->color);
	if (ft_strncmp(PLANE_ID, id, len) == 0)
		return (((t_plane *)object)->color);
	return (ft_rgb(0, 0, 0));
}

static void	put_pixel(t_mlx *mlx, t_fvector2 v, t_rgb rgb)
{
	if (v.x < 0 || v.x >= WIN_WIDTH || v.y < 0 || v.y >= WIN_HEIGHT)
		return ;
	*((unsigned int *)
			(mlx->data + (int)(v.y * mlx->size_line + v.x * (mlx->bpp / 8))))
		= (rgb.r << 16 | rgb.g << 8 | rgb.b);
}

// normalisation sur [-1, 1];
static t_fvector3	ray_tracer(t_camera *cam, t_fvector2 v)
{
	t_fvector3		ray_dir;
	float			norm_x;
	float			norm_y;

	norm_x = (2 * ((v.x + 0.5f) / WIN_WIDTH) - 1)
		* ((float)WIN_WIDTH / (float)WIN_HEIGHT) * cam->norm_fov;
	norm_y = (1 - 2 * ((v.y + 0.5f) / WIN_HEIGHT)) * cam->norm_fov;
	ray_dir = normalize(ft_fvector3(norm_x, norm_y, cam->norm_fov));
	return (ray_dir);
}
