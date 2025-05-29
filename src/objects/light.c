/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:04:39 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/29 12:39:16 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_light	*light(t_fvector3 position, float level, t_rgb color)
{
	t_light	*l;

	l = malloc(sizeof(t_light));
	if (!l)
		return (NULL);
	l->id = LIGHT_ID;
	l->position = position;
	l->level = level;
	l->color = color;
	l->render = get_render_by_id(LIGHT_ID);
	l->intersect = get_intersect_by_id(LIGHT_ID);
	l->is_inside = NULL;
	return (l);
}

void	*parse_light(char **values)
{
	t_fvector3	position;
	float		level;
	t_rgb		color;

	if (!values[0] || !values[1] || !values[2] || values[3])
		return (error_and_null(L_ARGS_E));
	if (!parse_fvector3(values[0], &position))
		return (error_and_null(L_POS_E));
	if (!ft_isnumeric(values[1]))
		return (error_and_null(L_LVL_E));
	level = ft_atof(values[1]);
	if (level < 0.0f || level > 1.0f)
		return (error_and_null(L_LVL_E));
	if (!parse_color(values[2], &color))
		return (error_and_null(L_RGB_E));
	return (light(position, level, color));
}
