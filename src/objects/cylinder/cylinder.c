/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:11:35 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/01 18:28:26 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_cylinder	*cylinder(t_fvector3 position, t_fvector3 normal,
	t_fvector2 size, t_pattern pattern)
{
	t_cylinder	*cy;
	t_fvector3	ref;

	cy = malloc(sizeof(t_cylinder));
	if (!cy)
		return (NULL);
	cy->id = CYLINDER_ID;
	cy->position = position;
	cy->normal = ft_fnormalize(normal);
	ref = (t_fvector3){0.0f, 1.0f, 0.0f};
	if (fabsf(ft_fdot_product(cy->normal, ref)) > 0.999f)
		ref = (t_fvector3){1.0f, 0.0f, 0.0f};
	cy->right = ft_fnormalize(ft_fcross_product(cy->normal, ref));
	cy->up = ft_fnormalize(ft_fcross_product(cy->right, cy->normal));
	cy->pattern = pattern;
	cy->diameter = size.x;
	cy->radius = size.x * 0.5f;
	cy->height = size.y;
	cy->half_height = size.y * 0.5f;
	cy->infinite = size.y < 0.0f;
	cy->methods = get_methods_by_id(CYLINDER_ID);
	cy->selected = 0;
	return (cy);
}

void	*parse_cylinder(char **values)
{
	t_fvector3	position;
	t_fvector3	normal;
	t_fvector2	size;
	t_pattern	pattern;

	if (!values[0] || !values[1] || !values[2] || !values[3] || !values[4]
		|| (values[5] && values[6] && values[7] && values[8] && values[9]))
		return (error_and_null(CY_ARGS_E));
	if (!parse_fvector3(values[0], &position))
		return (error_and_null(CY_POS_E));
	if (!parse_normal(values[1], &normal))
		return (error_and_null(CY_NORM_E));
	size.x = ft_atof(values[2]);
	if (!ft_isnumeric(values[2]) || ft_isoutint(values[2]) || size.x < 0.0f)
		return (error_and_null(CY_DIAM_E));
	size.y = ft_atof(values[3]);
	if (!ft_isnumeric(values[3]) || ft_isoutint(values[3])
		|| (size.y < 0.0f && size.y != -1.0f))
		return (error_and_null(CY_HEI_E));
	init_pattern(&pattern);
	if (!parse_texture(values[4], &pattern))
		return (error_and_null(CY_TEXTURE_E));
	if (values[5] && !parse_pattern(values + 5, &pattern))
		return (error_and_null(CY_ARGS_E));
	return (cylinder(position, normal, size, pattern));
}
