/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:49:15 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/17 09:56:39 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
		register_type(AMBIANT_ID, parse_ambiant, NULL);
		register_type(CAMERA_ID, parse_camera, NULL);
		register_type(CYLINDER_ID, parse_cylinder, NULL);
		register_type(LIGHT_ID, parse_light, NULL);
		register_type(PLANE_ID, parse_plane, render_plane);
		register_type(SPHERE_ID, parse_sphere, render_sphere);
	}
	return (mrt);
}
