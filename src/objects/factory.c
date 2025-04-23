/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factory.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:45:41 by lroussel          #+#    #+#             */
/*   Updated: 2025/04/23 12:02:45 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	register_object(char *id, t_object *object)
{
	t_minirt	*mrt;

	mrt = minirt();
	object->id = id;
	ft_lstadd_back(&mrt->objects, ft_lstnew(object));
}

t_object	*create_object(t_fvector3 position, t_fvector3 rotation, t_rgb rgb)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->position = position;
	obj->rotation = rotation;
	obj->rgb = rgb;
	return (obj);
}
