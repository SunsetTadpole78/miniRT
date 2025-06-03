/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:49:15 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/03 06:49:04 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void	register_types(void);
static inline void	init_thread_data(t_thread_data *data, int cores, int i,
						t_minirt *mrt);
/* -------------------------------------------------------------------------- */

t_minirt	*minirt(void)
{
	static t_minirt	*mrt = NULL;

	if (!mrt)
	{
		mrt = malloc(sizeof(t_minirt));
		if (!mrt)
			return (NULL);
		mrt->types = NULL;
		mrt->objects = NULL;
		mrt->lights = NULL;
		mrt->ambiant = NULL;
		mrt->camera = NULL;
		mrt->mlx = malloc(sizeof(t_mlx));
		if (!mrt->mlx)
			return (NULL);
		mrt->selected = NULL;
		mrt->ctrl_pressed = 0;
		mrt->clipboard = NULL;
		mrt->workers = 0;
		pthread_mutex_init(&mrt->workers_mutex, NULL);
		pthread_mutex_init(&mrt->update_mutex, NULL);
		register_types();
	}
	return (mrt);
}

static inline void	register_types(void)
{
	register_type(AMBIANT_ID, parse_ambiant, NULL, empty_methods());
	register_type(CAMERA_ID, parse_camera, NULL, empty_methods());
	register_type(CONE_ID, parse_cone, render_cone, init_methods(intersect_cone,
			is_inside_cone, on_press_key_cone, duplicate_cone));
	register_type(CYLINDER_ID, parse_cylinder, render_cylinder, init_methods(
			intersect_cylinder, is_inside_cylinder, on_press_key_cylinder,
			duplicate_cylinder));
	register_type(LIGHT_ID, parse_light, NULL, init_methods(NULL, NULL,
			on_press_key_light, duplicate_light));
	register_type(PLANE_ID, parse_plane, render_plane, init_methods(
			intersect_plane, NULL, on_press_key_plane, duplicate_plane));
	register_type(SPHERE_ID, parse_sphere, render_sphere, init_methods(
			intersect_sphere, is_inside_sphere, on_press_key_sphere,
			duplicate_sphere));
}

int	check_env(t_minirt *mrt)
{
	t_pattern		pattern;
	t_thread_data	*datas;
	int				cores;
	int				i;

	mrt->cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (mrt->cores == -1)
		return (ft_error(CORES_E, ERR_PREFIX, 0));
	if (!mrt->camera)
		return (ft_error(NEED_CAMERA_E, ERR_PREFIX, 0));
	if (!mrt->ambiant)
	{
		init_pattern(&pattern);
		mrt->ambiant = ambiant(0, pattern);
	}
	datas = malloc(sizeof(t_thread_data) * mrt->cores);
	if (!datas)
		return (0);
	cores = mrt->cores;
	mrt->pixels_per_thread = WIN_HEIGHT / cores;
	i = -1;
	while (++i < cores)
		init_thread_data(&datas[i], cores, i, mrt);
	mrt->threads_datas = datas;
	return (1);
}

inline void	init_thread_data(t_thread_data *data, int cores, int i,
		t_minirt *mrt)
{
	data->mrt = mrt;
	data->id = i + 1;
	data->start = WIN_HEIGHT / cores * i;
	if (i == (cores - 1))
		data->end = WIN_HEIGHT;
	else
		data->end = WIN_HEIGHT / cores * (i + 1);
	data->camera = data->mrt->camera;
	data->ratio = ((float)WIN_WIDTH / (float)WIN_HEIGHT)
		* data->camera->iplane_scale;
	data->update = 1;
	data->init = 0;
}

void	init_render(t_minirt *mrt)
{
	t_object	*cur;
	void		*ptr;
	t_pattern	*pattern;

	cur = mrt->objects;
	ptr = mrt->mlx->mlx_ptr;
	while (cur)
	{
		pattern = &cur->pattern;
		if (pattern->path)
		{
			pattern->texture.ptr = mlx_xpm_file_to_image(ptr, pattern->path,
					&pattern->texture.width, &pattern->texture.height);
			pattern->texture.data = mlx_get_data_addr(pattern->texture.ptr,
					&pattern->texture.bpp, &pattern->texture.ll,
					&pattern->texture.endian);
			pattern->texture.cl = pattern->texture.bpp / 8;
		}
		cur = cur->next;
	}
}
