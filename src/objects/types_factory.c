/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types_factory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:11:12 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/17 09:55:08 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	register_type(char *id, void *(*parser)(char **),
	void (*render)(t_ray *, t_fvector2, t_object *))
{
	t_type		*type;
	t_minirt	*mrt;

	if (exist_type(id))
		return (0);
	type = malloc(sizeof(t_type));
	if (!type)
		return (0);
	type->id = id;
	type->parser = parser;
	type->render = render;
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

void	*get_parser_by_id(char *id)
{
	t_type	*types;
	int		len;

	types = minirt()->types;
	len = ft_strlen(id) + 1;
	while (types)
	{
		if (ft_strncmp(types->id, id, len) == 0)
			return (types->parser);
		types = types->next;
	}
	return (NULL);
}

void	*get_render_by_id(char *id)
{
	t_type	*types;
	int		len;

	types = minirt()->types;
	len = ft_strlen(id) + 1;
	while (types)
	{
		if (ft_strncmp(types->id, id, len) == 0)
			return (types->render);
		types = types->next;
	}
	return (NULL);
}
