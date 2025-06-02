/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:52:34 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 12:09:48 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_object	*duplicate_plane(t_object *object)
{
	t_plane		*plane;
	t_plane		*new;

	plane = (t_plane *)object;
	new = malloc(sizeof(t_plane));
	if (!new)
		return (NULL);
	new->id = PLANE_ID;
	new->position = plane->position;
	new->normal = plane->normal;
	new->right = plane->right;
	new->up = plane->normal;
	new->pattern = plane->pattern;
	new->methods = plane->methods;
	new->selected = 0;
	return ((t_object *)new);
}
