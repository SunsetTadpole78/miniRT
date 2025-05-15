/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factory.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:45:41 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/15 09:56:03 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	register_object(void *object)
{
	char		*id;
	int			len;
	t_minirt	*mrt;

	if (!object)
		return (0);
	id = ((t_object *)object)->id;
	len = ft_strlen(id) + 1;
	if (ft_strncmp(AMBIANT_ID, id, len) == 0)
		return (set_ambiant((t_ambiant *)object));
	if (ft_strncmp(LIGHT_ID, id, len) == 0)
		return (register_light((t_light *)object));
	if (ft_strncmp(CAMERA_ID, id, len) == 0)
		return (set_camera((t_camera *)object));
	mrt = minirt();
	((t_object *)object)->next = mrt->objects;
	mrt->objects = (t_object *)object;
	return (1);
}

int	register_light(t_light *light)
{
	char		*id;
	t_minirt	*mrt;

	if (!light)
		return (0);
	id = light->id;
	if (ft_strncmp(LIGHT_ID, id, ft_strlen(id) + 1) != 0)
	{
		free(light);
		return (0);
	}
	mrt = minirt();
	light->next = (t_object *)mrt->lights;
	mrt->lights = light;
	return (1);
}

int	set_ambiant(t_ambiant *ambiant)
{
	t_minirt	*mrt;
	char		*id;

	if (!ambiant)
		return (0);
	id = ambiant->id;
	if (ft_strncmp(AMBIANT_ID, id, ft_strlen(id) + 1) != 0)
	{
		free(ambiant);
		return (0);
	}
	mrt = minirt();
	free(mrt->ambiant);
	mrt->ambiant = ambiant;
	return (1);
}

int	set_camera(t_camera *camera)
{
	t_minirt	*mrt;
	char		*id;

	if (!camera)
		return (0);
	id = camera->id;
	if (ft_strncmp(CAMERA_ID, id, ft_strlen(id) + 1) != 0)
	{
		free(camera);
		return (0);
	}
	mrt = minirt();
	free(mrt->camera);
	mrt->camera = camera;
	return (1);
}
