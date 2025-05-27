/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:11:35 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/27 15:53:08 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_cylinder	*cylinder(t_fvector3 position, t_fvector3 normal,
	t_fvector2 size, t_rgb color)
{
	t_cylinder	*cy;

	cy = malloc(sizeof(t_cylinder));
	if (!cy)
		return (NULL);
	cy->id = CYLINDER_ID;
	cy->position = position;
	cy->normal = normal;
	cy->size = size;
	cy->color = color;
	cy->render = get_render_by_id(CYLINDER_ID);
	return (cy);
}

void	*parse_cylinder(char **values)
{
	t_fvector3	position;
	t_fvector3	normal;
	t_fvector2	size;
	t_rgb		color;

	if (!values[0] || !values[1] || !values[2] || !values[3]
		|| !values[4] || values[5])
		return (error_and_null(CY_ARGS_E));
	if (!parse_fvector3(values[0], &position))
		return (error_and_null(CY_POS_E));
	if (!parse_normal(values[1], &normal))
		return (error_and_null(CY_NORM_E));
	if (!ft_isnumeric(values[2]) || ft_isoutint(values[2]))
		return (error_and_null(CY_DIAM_E));
	size.x = ft_atof(values[2]);
	if (size.x < 0.0f)
		return (error_and_null(CY_DIAM_E));
	if (!ft_isnumeric(values[3]) || ft_isoutint(values[3]))
		return (error_and_null(CY_HEI_E));
	size.y = ft_atof(values[3]);
	if (size.y < 0.0f)
		return (error_and_null(CY_HEI_E));
	if (!parse_color(values[4], &color))
		return (NULL);
	return (cylinder(position, normal, size, color));
}
