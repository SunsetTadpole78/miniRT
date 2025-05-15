/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
void		init_cam_and_plane(void);
/* -------------------------------------------------------------------------- */

void	init_cam_and_plane(void)
{
	t_minirt		*mrt;
	t_fvector3		pos;
	t_fvector3		norm;
	t_rgb			rgb;

	mrt = minirt();
	pos = ft_fvector3(-50.0f, 0.0f, 20.0f);
	norm = ft_fvector3(0.0f, 0.0f, 1.0f);
	mrt->camera = camera(pos, norm, 70);
	pos = ft_fvector3(0.0f, 0.0f, -10.0f);
	norm = ft_fvector3(0.0f, 1.0f, 0.0f);
	rgb = ft_rgb(255, 0, 0);
	mrt->plane = plane(pos, norm, rgb);
}

// aspect ratio
// scale
// imagex; imagey;
// direction
//ray.origin = cam->posit;
//ray.direction = normalize();
t_ray	get_ray(t_camera *cam, t_fvector2 uv)
{
	t_ray		ray;

	(void)ray.origin;
	(void)ray.direction;
	(void)uv;
	(void)cam;
	return (ray);
}
