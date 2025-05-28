/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types_factory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:11:12 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/28 18:48:58 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	register_type(char *id, t_methods *methods)
{
	t_type		*type;
	t_minirt	*mrt;

	if (exist_type(id))
		return (0);
	type = malloc(sizeof(t_type));
	if (!type)
		return (0);
	type->id = id;
	type->methods = methods;
	mrt = minirt();
	type->next = mrt->types;
	mrt->types = type;
	return (1);
}

int	exist_type(char *id)
{
	t_type	*types;
	int		len;

	types = minirt()->types;
	len = ft_strlen(id) + 1;
	while (types)
	{
		if (ft_strncmp(id, types->id, len) == 0)
			return (1);
		types = types->next;
	}
	return (0);
}

t_methods	*init_methods(void *(*parser)(char **),
		void (*render)(t_minirt *, t_ray *, t_object *, int depth),
		float (*intersect)(t_ray, t_object *),
		void (*on_press_key)(t_object *, int keycode, t_camera *camera))
{
	t_methods	*methods;

	methods = malloc(sizeof(t_methods));
	if (!methods)
		return (NULL);
	methods->parser = parser;
	methods->render = render;
	methods->intersect = intersect;
	methods->on_press_key = on_press_key;
	return (methods);
}

t_methods	*get_methods_by_id(char *id)
{
	t_type	*cur;
	int		len;

	cur = minirt()->types;
	len = ft_strlen(id) + 1;
	while (cur)
	{
		if (ft_strncmp(cur->id, id, len) == 0)
			return (cur->methods);
		cur = cur->next;
	}
	return (NULL);
}
