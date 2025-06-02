/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:43:01 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 12:10:13 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_object	*duplicate_sphere(t_object *object)
{
	t_sphere	*sphere;
	t_sphere	*new;

	sphere = (t_sphere *)object;
	new = malloc(sizeof(t_sphere));
	if (!new)
		return (NULL);
	new->id = SPHERE_ID;
	new->position = sphere->position;
	new->pattern = sphere->pattern;
	new->diameter = sphere->diameter;
	new->radius = sphere->radius;
	new->methods = sphere->methods;
	new->selected = 0;
	return ((t_object *)new);
}
