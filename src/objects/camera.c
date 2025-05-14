/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:06:06 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/14 12:31:40 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_camera	*camera(t_fvector3 position, t_fvector3 normal, int fov)
{
	t_camera	*c;

	c = malloc(sizeof(t_camera));
	if (!c)
		return (NULL);
	c->id = CAMERA_ID;
	c->position = position;
	c->normal = normal;
	c->fov = fov;
	return (c);
}

void	*parse_camera(char **values)
{
	(void)values;
	return (NULL);
}
