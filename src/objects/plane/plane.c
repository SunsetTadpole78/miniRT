/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:10:17 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/01 18:28:44 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_plane	*plane(t_fvector3 position, t_fvector3 normal, t_pattern pattern)
{
	t_plane		*pl;
	t_fvector3	ref;

	pl = malloc(sizeof(t_plane));
	if (!pl)
		return (NULL);
	pl->id = PLANE_ID;
	pl->position = position;
	pl->normal = ft_fnormalize(normal);
	ref = (t_fvector3){0.0f, 1.0f, 0.0f};
	if (fabsf(ft_fdot_product(pl->normal, ref)) > 0.999f)
		ref = (t_fvector3){1.0f, 0.0f, 0.0f};
	pl->right = ft_fnormalize(ft_fcross_product(pl->normal, ref));
	pl->up = ft_fcross_product(pl->right, pl->normal);
	pl->pattern = pattern;
	pl->methods = get_methods_by_id(PLANE_ID);
	pl->selected = 0;
	return (pl);
}

void	*parse_plane(char **values)
{
	t_fvector3	position;
	t_fvector3	normal;
	t_pattern	pattern;

	if (!values[0] || !values[1] || !values[2] || (values[3] && values[4]
			&& values[5] && values[6] && values[7]))
		return (error_and_null(PL_ARGS_E));
	if (!parse_fvector3(values[0], &position))
		return (error_and_null(PL_POS_E));
	if (!parse_normal(values[1], &normal))
		return (error_and_null(PL_NORM_E));
	init_pattern(&pattern);
	if (!parse_texture(values[2], &pattern))
		return (error_and_null(PL_TEXTURE_E));
	if (values[3] && !parse_pattern(values + 3, &pattern))
		return (error_and_null(PL_ARGS_E));
	return (plane(position, normal, pattern));
}
