/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:49:15 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/20 15:47:22 by lroussel         ###   ########.fr       */
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
		mrt->threads_init = 0;
		pthread_mutex_init(&mrt->exit_mutex, NULL);
		mrt->exit = 0;
		register_types();
	}
	return (mrt);
}

static inline void	register_types(void)
{
	register_type(AMBIANT_ID, parse_ambiant, NULL, empty_methods());
	register_type(CAMERA_ID, parse_camera, NULL, empty_methods());
	register_type(CONE_ID, parse_cone, intersect_cone, init_methods(
			apply_lights_cone, is_inside_cone, on_press_key_cone,
			duplicate_cone));
	register_type(CYLINDER_ID, parse_cylinder, intersect_cylinder, init_methods(
			apply_lights_cylinder, is_inside_cylinder, on_press_key_cylinder,
			duplicate_cylinder));
	register_type(LIGHT_ID, parse_light, NULL, init_methods(NULL, NULL,
			on_press_key_light, duplicate_light));
	register_type(PLANE_ID, parse_plane, intersect_plane, init_methods(
			apply_lights_plane, NULL, on_press_key_plane, duplicate_plane));
	register_type(SPHERE_ID, parse_sphere, intersect_sphere, init_methods(
			apply_lights_sphere, is_inside_sphere, on_press_key_sphere,
			duplicate_sphere));
}

int	check_env(t_minirt *mrt)
{
	t_pattern	pattern;
	int			cores;
	int			i;

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
	mrt->threads_datas = malloc(sizeof(t_thread_data) * mrt->cores);
	if (!mrt->threads_datas)
		return (0);
	cores = mrt->cores;
	mrt->pixels_per_thread = (WIN_WIDTH * WIN_HEIGHT) / cores;
	i = -1;
	while (++i < cores)
		init_thread_data(&mrt->threads_datas[i], cores, i, mrt);
	sem_unlink("/workers_sem");
	mrt->workers_sem = sem_open("/workers_sem", O_CREAT, 0644, cores - 1);
	return (1);
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

inline void	init_thread_data(t_thread_data *data, int cores, int i,
		t_minirt *mrt)
{
	int	pixels_per_thread;

	pixels_per_thread = mrt->pixels_per_thread;
	data->mrt = mrt;
	data->id = i;
	data->start = i * pixels_per_thread;
	if (i == cores - 1)
		data->end = WIN_WIDTH * WIN_HEIGHT;
	else
		data->end = (i + 1) * pixels_per_thread;
	data->camera = data->mrt->camera;
	data->ratio = ((float)WIN_WIDTH / (float)WIN_HEIGHT)
		* data->camera->iplane_scale;
	pthread_mutex_init(&data->update_mutex, NULL);
	pthread_cond_init(&data->update_cond, NULL);
}
