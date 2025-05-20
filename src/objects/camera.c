/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:06:06 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/19 11:37:37 by lroussel         ###   ########.fr       */
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
	c->right = ft_fnormalize(ft_fcross_product(c->normal,
				(t_fvector3){0, 1, 0}));
	c->up = ft_fcross_product(c->right, c->normal);
	c->fov = fov;
	c->iplane_scale = tan((fov / 2) * (M_PI / 180.0f));
	c->render = get_render_by_id(CAMERA_ID);
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

void	update_yaw(t_camera *cam, float theta)
{
	float		cos_t;
	float		sin_t;
	t_fvector3	world;

	world = (t_fvector3){0, 1, 0};
	if (ft_fdot_product(cam->up, world) < 0)
	{
		theta = -theta;
		world = (t_fvector3){0, -1, 0};
	}
	cos_t = cosf(theta);
	sin_t = sinf(theta);
	cam->normal = ft_fnormalize(
			(t_fvector3){cam->normal.x * cos_t - cam->normal.z * sin_t,
			cam->normal.y,
			cam->normal.x * sin_t + cam->normal.z * cos_t});
	cam->right = ft_fnormalize(ft_fcross_product(cam->normal, world));
	cam->up = ft_fcross_product(cam->right, cam->normal);
}

void	update_pitch(t_camera *cam, float theta)
{
	float		cos_t;
	float		sin_t;

	cos_t = cosf(theta);
	sin_t = sinf(theta);
	cam->normal = ft_fnormalize(
			(t_fvector3){cam->normal.x * cos_t + cam->up.x * sin_t,
			cam->normal.y * cos_t + cam->up.y * sin_t,
			cam->normal.z * cos_t + cam->up.z * sin_t});
	cam->up = ft_fnormalize(ft_fcross_product(cam->right, cam->normal));
}

void	update_fov(t_minirt *mrt, int incrementation)
{
	int	fov;

	fov = mrt->camera->fov;
	if ((fov == 0 && incrementation == -1)
		|| (fov == 180 && incrementation == 1))
		return ;
	mrt->camera->fov += incrementation;
	mrt->camera->iplane_scale = tan((mrt->camera->fov / 2) * (M_PI / 180.0f));
}
