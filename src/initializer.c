/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:49:15 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 01:56:05 by lroussel         ###   ########.fr       */
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
		register_types();
	}
	return (mrt);
}

static inline void	register_types(void)
{
	register_type(AMBIANT_ID, parse_ambiant, empty_methods());
	register_type(CAMERA_ID, parse_camera, empty_methods());
	register_type(CONE_ID, parse_cone, init_methods(render_cone, intersect_cone,
			is_inside_cone, on_press_key_cone));
	register_type(CYLINDER_ID, parse_cylinder, init_methods(render_cylinder,
			intersect_cylinder, is_inside_cylinder, on_press_key_cylinder));
	register_type(LIGHT_ID, parse_light, init_methods(NULL, NULL, NULL,
			on_press_key_light));
	register_type(PLANE_ID, parse_plane, init_methods(render_plane,
			intersect_plane, NULL, on_press_key_plane));
	register_type(SPHERE_ID, parse_sphere, init_methods(render_sphere,
			intersect_sphere, is_inside_sphere, on_press_key_sphere));
}

int	check_env(t_minirt *mrt)
{
	t_thread_data	*datas;
	int				cores;
	int				i;

	mrt->cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (mrt->cores == -1)
		return (ft_error(CORES_E, ERR_PREFIX, 0));
	if (!mrt->camera)
		return (ft_error(NEED_CAMERA_E, ERR_PREFIX, 0));
	if (!mrt->ambiant)
		mrt->ambiant = ambiant(0, (t_rgb){0, 0, 0});
	datas = malloc(sizeof(t_thread_data) * mrt->cores);
	if (!datas)
		return (0);
	cores = mrt->cores;
	mrt->pixels_per_thread = (WIN_WIDTH * WIN_HEIGHT) / cores;
	i = 0;
	while (i < cores)
	{
		init_thread_data(&datas[i], cores, i, mrt);
		i++;
	}
	mrt->threads_datas = datas;
	return (1);
}

inline void	init_thread_data(t_thread_data *data, int cores, int i,
		t_minirt *mrt)
{
	int	pixels_per_thread;

	pixels_per_thread = mrt->pixels_per_thread;
	data->mrt = mrt;
	data->start = i * pixels_per_thread;
	if (i == cores - 1)
		data->end = WIN_WIDTH * WIN_HEIGHT;
	else
		data->end = (i + 1) * pixels_per_thread;
	data->camera = data->mrt->camera;
	data->ratio = ((float)WIN_WIDTH / (float)WIN_HEIGHT)
		* data->camera->iplane_scale;
}
