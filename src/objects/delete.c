/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:10:01 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 13:17:17 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void	check_lights(t_minirt *mrt, t_object *object);
/* -------------------------------------------------------------------------- */

void	delete_object(t_minirt *mrt, t_object *object)
{
	t_object	*previous;
	t_object	*cur;
	int			finded;

	previous = NULL;
	cur = mrt->objects;
	finded = 0;
	while (cur)
	{
		if (cur == object)
		{
			if (previous)
				previous->next = cur->next;
			else
				mrt->objects = cur->next;
			finded = 1;
			break ;
		}
		previous = cur;
		cur = cur->next;
	}
	if (!finded)
		check_lights(mrt, object);
	free(object);
	mrt->selected = NULL;
}

static inline void	check_lights(t_minirt *mrt, t_object *object)
{
	t_object	*previous;
	t_object	*cur;

	previous = NULL;
	cur = (t_object *)mrt->lights;
	while (cur)
	{
		if (cur == object)
		{
			if (previous)
				previous->next = cur->next;
			else
				mrt->lights = (t_light *)cur->next;
			break ;
		}
		previous = cur;
		cur = cur->next;
	}
}
