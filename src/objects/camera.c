/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:06:06 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/17 18:46:21 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_camera	*camera(t_fvector3 position, t_fvector3 normal, int fov)
{
	t_camera	*c;

	c = malloc(sizeof(t_camera));
	if (!c)
		return (NULL);
	c->id = CAMERA_ID;
	c->position = position;
	c->normal = normalize(normal);
	c->fov = fov;
	c->image_plane_scale = tan((fov / 2) * (M_PI / 180.0f));
	c->world = mat4_look_at(position,
			sub_vectors(position, c->normal), (t_fvector3){0, 1, 0});
	return (c);
}

void	*parse_camera(char **values)
{
	t_fvector3	position;
	t_fvector3	normal;
	int			fov;

	if (!values[0] || !values[1] || !values[2] || values[3])
		return (error_and_null(C_ARGS_E));
	if (!parse_fvector3(values[0], &position, C_POS_E)
		|| !parse_normal(values[1], &normal, C_NORM_E))
		return (NULL);
	if (!ft_isnumeric(values[2]) || ft_isoutint(values[2]))
		return (error_and_null(C_FOV_E));
	fov = ft_atoi(values[2]);
	if (fov < 0 || fov > 180)
		return (error_and_null(C_FOV_E));
	return (camera(position, normal, fov));
}
