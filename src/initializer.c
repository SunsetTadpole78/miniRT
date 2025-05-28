/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:49:15 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/28 02:05:00 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_minirt	*minirt(void)
{
	static t_minirt	*mrt = NULL;

	if (!mrt)
	{
		mrt = malloc(sizeof(t_minirt));
		if (!mrt)
			return (NULL);
		mrt->types = NULL;
		mrt->objects = NULL;
		mrt->lights = NULL;
		mrt->ambiant = NULL;
		mrt->camera = NULL;
		mrt->mlx = malloc(sizeof(t_mlx));
		if (!mrt->mlx)
			return (NULL);
		register_type(AMBIANT_ID, parse_ambiant, NULL, NULL);
		register_type(CAMERA_ID, parse_camera, NULL, NULL);
		register_type(CYLINDER_ID, parse_cylinder, render_cylinder,
			intersect_cylinder);
		register_type(LIGHT_ID, parse_light, NULL, NULL);
		register_type(PLANE_ID, parse_plane, render_plane, intersect_plane);
		register_type(SPHERE_ID, parse_sphere, render_sphere, intersect_sphere);
	}
	return (mrt);
}

int	check_env(t_minirt *mrt)
{
	mrt->cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (mrt->cores == -1)
		return (ft_error(CORES_E, ERR_PREFIX, 0));
	if (!mrt->camera)
		return (ft_error(NEED_CAMERA_E, ERR_PREFIX, 0));
	if (!mrt->ambiant)
		mrt->ambiant = ambiant(0, (t_rgb){0, 0, 0});
	return (1);
}
