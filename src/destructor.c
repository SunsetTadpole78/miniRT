/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:31:04 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/26 11:23:21 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* -------------------------------- PROTOTYPE ------------------------------- */
static void	free_objects(t_object *objects, void *mlx_ptr);
static void	free_types(t_type *types);
/* -------------------------------------------------------------------------- */

void	destruct_minirt(t_minirt *mrt, int destroy_mlx)
{
	t_mlx	*mlx;

	mlx = mrt->mlx;
	free_types(mrt->types);
	free_objects(mrt->objects, mlx->mlx_ptr);
	free_objects((t_object *)mrt->lights, mlx->mlx_ptr);
	free(mrt->ambiant);
	free(mrt->camera);
	free(mrt->clipboard);
	if (destroy_mlx)
		destruct_mlx(mlx);
	free(mlx);
	if (mrt->threads_init)
	{
		stop_threads(mrt);
		free(mrt->threads_datas);
	}
	pthread_mutex_destroy(&mrt->exit_mutex);
	sem_close(mrt->workers_sem);
	sem_unlink("/workers_sem");
	free(mrt);
}

static void	free_objects(t_object *objects, void *mlx_ptr)
{
	t_object	*tmp;

	while (objects)
	{
		tmp = objects->next;
		free(objects->pattern.path);
		if (objects->pattern.texture.ptr)
			mlx_destroy_image(mlx_ptr, objects->pattern.texture.ptr);
		free(objects->pattern.bump_path);
		if (objects->pattern.texture.ptr)
		if (objects->pattern.bump.ptr)
			mlx_destroy_image(mlx_ptr, objects->pattern.bump.ptr);
		free(objects);
		objects = tmp;
	}
}

static void	free_types(t_type *types)
{
	t_type	*tmp;

	while (types)
	{
		tmp = types->next;
		free(types->methods);
		free(types);
		types = tmp;
	}
}
