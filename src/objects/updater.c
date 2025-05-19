/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   updater.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 10:05:33 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/19 10:35:20 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	update_values(t_minirt *mrt)
{
	t_object	*object;
	void		(*updater)(t_minirt *, t_object *);

	if (!mrt->ambiant)
		mrt->ambiant = ambiant(0, ft_rgb(0, 0, 0));
	object = mrt->objects;
	while (object)
	{
		updater = get_updater_by_id(object->id);
		if (updater)
			updater(mrt, object);
		object = object->next;
	}
}

void	update_object_colors(t_minirt *mrt, t_object *object)
{
	t_color_object	*colored;
	t_rgb			base;
	t_ambiant		*ambiant;
	float			level;

	colored = (t_color_object *)object;
	base = colored->color;
	ambiant = mrt->ambiant;
	level = ambiant->level;
	colored->color.r = (base.r + ((level / 2.0f)
				* (ambiant->color.r - base.r))) * level;
	colored->color.g = (base.g + ((level / 2.0f)
				* (ambiant->color.g - base.g))) * level;
	colored->color.b = (base.b + ((level / 2.0f)
				* (ambiant->color.b - base.b))) * level;
}
