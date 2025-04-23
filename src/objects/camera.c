/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:06:06 by lroussel          #+#    #+#             */
/*   Updated: 2025/04/23 13:07:11 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_camera	*camera(t_fvector3 position, t_fvector3 normal, int fov)
{
	t_camera	*c;

	c = malloc(sizeof(t_camera));
	if (!c)
		return (NULL);
	c->position = position;
	c->normal = normal;
	c->fov = fov;
	return (c);
}
