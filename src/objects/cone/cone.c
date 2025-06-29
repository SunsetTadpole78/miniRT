/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:20:44 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/01 18:28:31 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_fvector3	get_right_vector(t_fvector3 normal);
/* -------------------------------------------------------------------------- */

t_cone	*cone(t_fvector3 position, t_fvector3 normal, t_fvector2 size,
		t_pattern pattern)
{
	t_cone	*co;

	co = malloc(sizeof(t_cone));
	if (!co)
		return (NULL);
	co->id = CONE_ID;
	co->position = position;
	co->normal = ft_fnormalize(normal);
	co->right = get_right_vector(normal);
	co->up = ft_fcross_product(co->normal, co->right);
	co->pattern = pattern;
	co->base_diameter = size.x;
	co->radius = size.x * 0.5f;
	co->infinite = size.y < 0.0f;
	if (co->infinite)
		size.y = size.x * 0.6f;
	co->height = size.y;
	co->k = (size.x * 0.5f) / fabsf(size.y);
	co->k2 = co->k * co->k;
	co->methods = get_methods_by_id(CONE_ID);
	co->selected = 0;
	return (co);
}

void	*parse_cone(char **values)
{
	t_fvector3	position;
	t_fvector3	normal;
	t_fvector2	size;
	t_pattern	pattern;

	if (!values[0] || !values[1] || !values[2] || !values[3] || !values[4]
		|| (values[5] && values[6] && values[7] && values[8] && values[9]))
		return (error_and_null(CO_ARGS_E));
	if (!parse_fvector3(values[0], &position))
		return (error_and_null(CO_POS_E));
	if (!parse_normal(values[1], &normal))
		return (error_and_null(CO_NORM_E));
	size.x = ft_atof(values[2]);
	if (!ft_isnumeric(values[2]) || ft_isoutint(values[2]) || size.x < 0.0f)
		return (error_and_null(CO_DIAM_E));
	size.y = ft_atof(values[3]);
	if (!ft_isnumeric(values[3]) || ft_isoutint(values[3])
		|| (size.y < 0.0f && size.y != -1.0f))
		return (error_and_null(CO_HEI_E));
	init_pattern(&pattern);
	if (!parse_texture(values[4], &pattern))
		return (error_and_null(CO_TEXTURE_E));
	if (values[5] && !parse_pattern(values + 5, &pattern))
		return (error_and_null(CO_ARGS_E));
	return (cone(position, normal, size, pattern));
}

static inline t_fvector3	get_right_vector(t_fvector3 normal)
{
	if (fabsf(normal.y) < 0.999f)
		return (ft_fnormalize(ft_fcross_product(
					(t_fvector3){0.0f, 1.0f, 0.0f}, normal)));
	return (ft_fnormalize(ft_fcross_product(
				(t_fvector3){1.0f, 0.0f, 0.0f}, normal)));
}
