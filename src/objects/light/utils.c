/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:06:23 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 12:09:23 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_object	*duplicate_light(t_object *object)
{
	t_light	*light;
	t_light	*new;

	light = (t_light *)object;
	new = malloc(sizeof(t_light));
	if (!new)
		return (NULL);
	new->id = LIGHT_ID;
	new->position = light->position;
	new->level = light->level;
	new->color = light->color;
	new->scale = light->scale;
	new->methods = light->methods;
	new->selected = 0;
	new->radius = light->radius;
	new->visible = light->visible;
	return ((t_object *)new);
}
