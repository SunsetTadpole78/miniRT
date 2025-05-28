/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:31:04 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/28 18:40:55 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* -------------------------------- PROTOTYPE ------------------------------- */
static void	free_objects(t_object *objects);
static void	free_types(t_type *type);
/* -------------------------------------------------------------------------- */

void	destruct_minirt(t_minirt *mrt, int destroy_mlx)
{
	t_mlx		*mlx;

	mlx = mrt->mlx;
	free_types(mrt->types);
	free_objects(mrt->objects);
	free_objects((t_object *)mrt->lights);
	free(mrt->ambiant);
	free(mrt->camera);
	if (destroy_mlx)
		destruct_mlx(mlx);
	free(mlx);
	free(mrt);
}

static void	free_objects(t_object *objects)
{
	t_object	*tmp;

	while (objects)
	{
		tmp = objects->next;
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
