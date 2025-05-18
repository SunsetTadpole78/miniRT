/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:06:06 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/17 20:59:16 by lroussel         ###   ########.fr       */
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
	c->normal = ft_fnormalize(normal);
	c->fov = fov;
	c->iplane_scale = tan((fov / 2) * (M_PI / 180.0f));
	c->right = ft_fnormalize(ft_fcross_product(c->normal,
				(t_fvector3){0, 1, 0}));
	c->up = ft_fcross_product(c->right, c->normal);
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

void	rotate_camera_y(t_camera *cam, float theta)
{
	t_fvector3		new;
	float			cos_t;
	float			sin_t;

	cos_t = cosf(theta);
	sin_t = sinf(theta);
	new.x = cam->normal.x * cos_t - cam->normal.z * sin_t;
	new.y = cam->normal.y;
	new.z = cam->normal.x * sin_t + cam->normal.z * cos_t;
	cam->normal = ft_fnormalize(new);
	cam->right = ft_fnormalize(ft_fcross_product(cam->normal,
				(t_fvector3){0, 1, 0}));
	cam->up = ft_fcross_product(cam->right, cam->normal);
}

void	add_fov(t_minirt *mrt, int incrementation)
{
	int	fov;

	fov = mrt->camera->fov;
	if ((fov == 0 && incrementation == -1)
		|| (fov == 180 && incrementation == 1))
		return ;
	mrt->camera->fov += incrementation;
	mrt->camera->iplane_scale = tan((mrt->camera->fov / 2) * (M_PI / 180.0f));
}
