/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:10:17 by lroussel          #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_plane	*plane(t_fvector3 position, t_fvector3 normal, t_rgb color)
{
	t_plane	*pl;

	pl = malloc(sizeof(t_plane));
	if (!pl)
		return (NULL);
	pl->id = PLANE_ID;
	pl->position = position;
	pl->normal = ft_fnormalize(normal);
	pl->color = color;
	pl->render = get_render_by_id(PLANE_ID);
	pl->smoothness = 1.0f;
	pl->mat = 0.7f;
	return (pl);
}

void	*parse_plane(char **values)
{
	t_fvector3	position;
	t_fvector3	normal;
	t_rgb		color;

	if (!values[0] || !values[1] || !values[2] || values[3])
		return (error_and_null(PL_ARGS_E));
	if (!parse_fvector3(values[0], &position, PL_POS_E)
		|| !parse_normal(values[1], &normal, PL_NORM_E)
		|| !parse_color(values[2], &color, PL_RGB_E))
		return (NULL);
	return (plane(position, normal, color));
}
