/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambiant.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:58:48 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/26 12:20:44 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_ambiant	*ambiant(float level, t_rgb color)
{
	t_ambiant	*a;

	a = malloc(sizeof(t_ambiant));
	if (!a)
		return (NULL);
	a->id = AMBIANT_ID;
	a->level = level;
	a->color.r = color.r * level;
	a->color.g = color.g * level;
	a->color.b = color.b * level;
	a->render = get_render_by_id(AMBIANT_ID);
	a->intersect = get_intersect_by_id(AMBIANT_ID);
	return (a);
}

void	*parse_ambiant(char **values)
{
	float	level;
	t_rgb	color;

	if (!values[0] || !values[1] || values[2])
		return (error_and_null(A_ARGS_E));
	if (!ft_isnumeric(values[0]))
		return (error_and_null(A_LVL_E));
	level = ft_atof(values[0]);
	if (level < 0.0f || level > 1.0f)
		return (error_and_null(A_LVL_E));
	if (!parse_color(values[1], &color, A_RGB_E))
		return (NULL);
	return (ambiant(level, color));
}
