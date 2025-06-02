/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambiant.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:58:48 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 13:43:09 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_ambiant	*ambiant(float level, t_pattern pattern)
{
	t_ambiant	*a;

	a = malloc(sizeof(t_ambiant));
	if (!a)
		return (NULL);
	a->id = AMBIANT_ID;
	a->level = level;
	pattern.main_color.r *= level;
	pattern.main_color.g *= level;
	pattern.main_color.b *= level;
	a->pattern = pattern;
	a->gamma_color.r = (float)powf(pattern.main_color.r / 255.0f, GAMMA)
		* level;
	a->gamma_color.g = (float)powf(pattern.main_color.g / 255.0f, GAMMA)
		* level;
	a->gamma_color.b = (float)powf(pattern.main_color.b / 255.0f, GAMMA)
		* level;
	a->methods = get_methods_by_id(AMBIANT_ID);
	a->selected = 0;
	return (a);
}

void	*parse_ambiant(char **values)
{
	float		level;
	t_pattern	pattern;

	if (!values[0] || !values[1] || values[2])
		return (error_and_null(A_ARGS_E));
	if (!ft_isnumeric(values[0]))
		return (error_and_null(A_LVL_E));
	level = ft_atof(values[0]);
	if (level < 0.0f || level > 1.0f)
		return (error_and_null(A_LVL_E));
	init_pattern(&pattern);
	if (!parse_color(values[1], &pattern.main_color))
		return (error_and_null(A_RGB_E));
	return (ambiant(level, pattern));
}
