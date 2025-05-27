/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:07:44 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/27 15:53:45 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_sphere	*sphere(t_fvector3 position, float diameter, t_pattern pattern)
{
	t_sphere	*sp;

	sp = malloc(sizeof(t_sphere));
	if (!sp)
		return (NULL);
	sp->id = SPHERE_ID;
	sp->position = position;
	sp->pattern = pattern;
	sp->diameter = diameter;
	sp->radius = diameter / 2.0f;
	sp->render = get_render_by_id(SPHERE_ID);
	return (sp);
}

void	*parse_sphere(char **values)
{
	t_fvector3	position;
	float		diameter;
	t_pattern	pattern;

	if (!values[0] || !values[1] || !values[2] || (values[3] && values[4] && values[5] && values[6] && values[7]))
		return (error_and_null(SP_ARGS_E));
	if (!parse_fvector3(values[0], &position))
		return (error_and_null(SP_POS_E));
	if (!ft_isnumeric(values[1]) || ft_isoutint(values[1]))
		return (error_and_null(SP_DIAM_E));
	diameter = ft_atof(values[1]);
	if (diameter < 0.0f)
		return (error_and_null(SP_DIAM_E));
	if (!parse_color(values[2], &pattern.main_color))
		return (NULL);
	if (values[3])
	{
		if (!parse_pattern(values + 3, &pattern))
			return (error_and_null(SP_ARGS_E));
	}
	else
	{
		pattern.id = '\0';
		pattern.secondary_color = (t_rgb){0,0,0};
		pattern.smoothness = 1.0f;
		pattern.mattifying = 0.0f;
	}
	return (sphere(position, diameter, pattern));
}
