/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factory.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:45:41 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/13 17:26:39 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	register_object(void *object)
{
	char		*id;
	int			len;

	if (!object)
		return (0);
	id = (char *)((t_identifiable *)object)->id;
	len = ft_strlen(id);
	if (ft_strncmp(AMBIANT_ID, id, len) == 0
		|| ft_strncmp(LIGHT_ID, id, len) == 0
		|| ft_strncmp(CAMERA_ID, id, len) == 0)
	{
		free(object);
		return (0);
	}
	ft_lstadd_back(&(minirt()->objects), ft_lstnew(object));
	return (1);
}

int	register_light(t_light *light)
{
	char		*id;
	int			len;

	if (!light)
		return (0);
	id = light->id;
	len = ft_strlen(id);
	if (ft_strncmp(LIGHT_ID, id, len) != 0)
	{
		free(light);
		return (0);
	}
	ft_lstadd_back(&(minirt()->lights), ft_lstnew(light));
	return (1);
}

int	set_ambiant(t_ambiant *ambiant)
{
	t_minirt	*mrt;
	char		*id;
	int			len;

	if (!ambiant)
		return (0);
	id = ambiant->id;
	len = ft_strlen(id);
	if (ft_strncmp(AMBIANT_ID, id, len) != 0)
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
	int			len;

	if (!camera)
		return (0);
	id = camera->id;
	len = ft_strlen(id);
	if (ft_strncmp(CAMERA_ID, id, len) != 0)
	{
		free(camera);
		return (0);
	}
	mrt = minirt();
	free(mrt->camera);
	mrt->camera = camera;
	return (1);
}
