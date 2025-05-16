/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:04:39 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/16 02:55:03 by lroussel         ###   ########.fr       */
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
	return (l);
}

void	*parse_light(char **values)
{
	t_fvector3	position;
	float		level;
	t_rgb		color;

	if (!values[0] || !values[1] || !values[2] || values[3])
		return (error_and_null(L_ARGS_E));
	if (!parse_fvector3(values[0], &position, L_POS_E))
		return (NULL);
	level = ft_atof(values[1]);
	if (level < 0.0f || level > 1.0f)
		return (error_and_null(L_LVL_E));
	if (!parse_color(values[2], &color, L_RGB_E))
		return (NULL);
	return (light(position, level, color));
}
