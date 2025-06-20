/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 13:35:52 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/28 17:08:11 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void		update_yaw(t_camera *camera, float theta);
static inline void		update_pitch(t_camera *camera, float theta);
static inline void		update_fov(t_camera *camera, int incrementation);
static inline t_fvector3	sum_inversed(t_fvector3 position,
							t_fvector3 normal);
/* -------------------------------------------------------------------------- */

void	on_press_key_camera(t_camera *camera, int keycode)
{
	if (keycode == OGLK_A || keycode == XK_A)
		camera->position = ft_fvector3_sum(camera->position, camera->right);
	else if (keycode == OGLK_S || keycode == XK_S)
		camera->position = sum_inversed(camera->position, camera->normal);
	else if (keycode == OGLK_D || keycode == XK_D)
		camera->position = sum_inversed(camera->position, camera->right);
	else if (keycode == OGLK_W || keycode == XK_W)
		camera->position = ft_fvector3_sum(camera->position, camera->normal);
	else if (keycode == OGLK_EQUAL || keycode == XK_EQUAL)
		update_fov(camera, -1);
	else if (keycode == OGLK_MINUS || keycode == XK_MINUS)
		update_fov(camera, 1);
	else if (keycode == OGLK_SPACE || keycode == XK_SPACE)
		camera->position.y += 1.0f;
	else if (keycode == OGLK_SHIFT || keycode == XK_SHIFT)
		camera->position.y -= 1.0f;
	else if (keycode == OGLK_ARROW_LEFT || keycode == XK_ARROW_LEFT)
		update_yaw(camera, 0.1f);
	else if (keycode == OGLK_ARROW_RIGHT || keycode == XK_ARROW_RIGHT)
		update_yaw(camera, -0.1f);
	else if (keycode == OGLK_ARROW_DOWN || keycode == XK_ARROW_DOWN)
		update_pitch(camera, -0.1f);
	else if (keycode == OGLK_ARROW_UP || keycode == XK_ARROW_UP)
		update_pitch(camera, 0.1f);
}

static inline void	update_yaw(t_camera *camera, float theta)
{
	float		cos_t;
	float		sin_t;
	t_fvector3	world;

	world = (t_fvector3){0.0f, 1.0f, 0.0f};
	if (ft_fdot_product(camera->up, world) <= EPSILON)
	{
		theta = -theta;
		world.y = -1.0f;
	}
	cos_t = cosf(theta);
	sin_t = sinf(theta);
	camera->normal = ft_fnormalize(
			(t_fvector3){camera->normal.x * cos_t - camera->normal.z * sin_t,
			camera->normal.y,
			camera->normal.x * sin_t + camera->normal.z * cos_t});
	camera->right = ft_fnormalize(ft_fcross_product(camera->normal, world));
	camera->up = ft_fcross_product(camera->right, camera->normal);
}

static inline void	update_pitch(t_camera *camera, float theta)
{
	float	cos_t;
	float	sin_t;

	cos_t = cosf(theta);
	sin_t = sinf(theta);
	camera->normal = ft_fnormalize(
			(t_fvector3){camera->normal.x * cos_t + camera->up.x * sin_t,
			camera->normal.y * cos_t + camera->up.y * sin_t,
			camera->normal.z * cos_t + camera->up.z * sin_t});
	camera->up = ft_fnormalize(ft_fcross_product(camera->right,
				camera->normal));
}

static inline void	update_fov(t_camera *camera, int incrementation)
{
	int	fov;

	fov = camera->fov;
	if ((fov == 0 && incrementation == -1)
		|| (fov == 180 && incrementation == 1))
		return ;
	camera->fov += incrementation;
	camera->iplane_scale = tan((camera->fov / 2) * (M_PI / 180.0f));
}

static inline t_fvector3	sum_inversed(t_fvector3 position, t_fvector3 normal)
{
	return (ft_fvector3_sum(position, ft_fvector3_scale(normal, -1.0f)));
}
