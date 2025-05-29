/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types_factory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:11:12 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/30 00:14:15 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	register_type(char *id, void *(*parser)(char **), t_methods *methods)
{
	t_type		*type;
	t_minirt	*mrt;

	if (exist_type(id))
		return (0);
	type = malloc(sizeof(t_type));
	if (!type)
		return (0);
	type->id = id;
	methods->parser = parser;
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

t_methods	*init_methods(void (*render)(t_minirt *, t_ray *, t_object *, int),
		float (*intersect)(t_ray *, t_object *, float),
		int (*is_inside)(t_object *, t_fvector3),
		void (*on_press_key)(t_object *, int, t_camera *))
{
	t_methods	*methods;

	methods = malloc(sizeof(t_methods));
	if (!methods)
		return (NULL);
	methods->parser = NULL;
	methods->render = render;
	methods->intersect = intersect;
	methods->is_inside = is_inside;
	methods->on_press_key = on_press_key;
	return (methods);
}

t_methods	*empty_methods(void)
{
	t_methods	*methods;

	methods = malloc(sizeof(t_methods));
	if (!methods)
		return (NULL);
	methods->parser = NULL;
	methods->render = NULL;
	methods->intersect = NULL;
	methods->is_inside = NULL;
	methods->on_press_key = NULL;
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
