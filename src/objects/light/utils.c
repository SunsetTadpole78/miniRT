/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:06:23 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/26 18:40:24 by lroussel         ###   ########.fr       */
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
	new->pattern = light->pattern;
	new->scale = light->scale;
	new->methods = light->methods;
	new->selected = 0;
	new->radius = light->radius;
	new->visible = light->visible;
	return ((t_object *)new);
}

int	is_light_inside(int oid, t_light *light)
{
	t_list	*cur;

	cur = light->inside;
	while (cur)
	{
		if (*((int *)cur->content) == oid)
			return (1);
		cur = cur->next;
	}
	return (0);
}

void	refresh_inside_lights(t_minirt *mrt)
{
	t_object	*cur;
	int			(*is_inside)(t_object *, t_fvector3);
	t_light		*light;

	light = mrt->lights;
	while (light)
	{
		ft_lstclear(&light->inside, ft_nothing);
		cur = mrt->objects;
		while (cur)
		{
			is_inside = cur->methods->is_inside;
			if (is_inside)
			{
				if (is_inside(cur, light->position))
					ft_lstadd_front(&light->inside, ft_lstnew(&cur->oid));
			}
			cur = cur->next;
		}
		light = (t_light *)light->next;
	}
}
