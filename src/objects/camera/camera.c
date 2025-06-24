/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:06:06 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/29 14:28:03 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
void	normalize_in_world(t_fvector3 normal, t_fvector3 *cnormal,
			t_fvector3 *up);
/* -------------------------------------------------------------------------- */

t_camera	*camera(t_fvector3 position, t_fvector3 normal, int fov)
{
	t_camera	*c;
	t_fvector3	ref;

	c = malloc(sizeof(t_camera));
	if (!c)
		return (NULL);
	c->id = CAMERA_ID;
	c->position = position;
	c->normal = ft_fnormalize(normal);
	ref = (t_fvector3){0.0f, 1.0f, 0.0f};
	if (fabsf(ft_fdot_product(c->normal, ref)) > 0.999f)
		ref = (t_fvector3){1.0f, 0.0f, 0.0f};
	c->right = ft_fnormalize(ft_fcross_product(c->normal, ref));
	c->up = ft_fcross_product(c->right, c->normal);
	normalize_in_world(normal, &c->normal, &c->up);
	c->fov = fov;
	c->iplane_scale = tan((fov / 2) * (M_PI / 180.0f));
	c->methods = get_methods_by_id(CAMERA_ID);
	c->selected = 0;
	return (c);
}

void	normalize_in_world(t_fvector3 normal, t_fvector3 *cnormal,
	t_fvector3 *up)
{
	if (normal.y != 0.0f && (normal.x <= 0.0f || normal.z <= 0.0f))
	{
		cnormal->x = EPSILON;
		up->y = EPSILON;
		if (normal.y < 0.0f)
		{
			cnormal->x = -EPSILON;
			up->y = -EPSILON;
		}
		if ((normal.x < 0.0f && normal.y < 0.0f)
			|| (normal.x > 0.0f && normal.y > 0.0f))
		{
			cnormal->x = -cnormal->x;
			up->y = -up->y;
		}
		if (normal.z != 0.0f)
			up->y += 0.3f;
	}
}

void	*parse_camera(char **values)
{
	t_fvector3	position;
	t_fvector3	normal;
	int			fov;

	if (!values[0] || !values[1] || !values[2] || values[3])
		return (error_and_null(C_ARGS_E));
	if (!parse_fvector3(values[0], &position))
		return (error_and_null(C_POS_E));
	if (!parse_normal(values[1], &normal))
		return (error_and_null(C_NORM_E));
	if (!ft_isnumeric(values[2]) || ft_isoutint(values[2]))
		return (error_and_null(C_FOV_E));
	fov = ft_atoi(values[2]);
	if (fov < 0 || fov > 180)
		return (error_and_null(C_FOV_E));
	return (camera(position, normal, fov));
}
