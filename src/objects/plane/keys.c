/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:08:17 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/28 18:39:26 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void	on_press_key_rotate(t_plane *plane, int keycode);
static inline void	rotate_plane_yaw(t_plane *plane, float theta);
static inline void	rotate_plane_pitch(t_plane *plane, float theta);
/* -------------------------------------------------------------------------- */

void	on_press_key_plane(t_object *object, int keycode, t_camera *camera)
{
	t_plane	*plane;

	plane = (t_plane *)object;
	if (keycode == OGLK_A || keycode == XK_A)
		plane->position = ft_fvector3_sum(plane->position, camera->right);
	else if (keycode == OGLK_S || keycode == XK_S)
		plane->position = ft_fvector3_sum(plane->position,
				ft_fvector3_scale(camera->normal, -1.0f));
	else if (keycode == OGLK_D || keycode == XK_D)
		plane->position = ft_fvector3_sum(plane->position,
				ft_fvector3_scale(camera->right, -1.0f));
	else if (keycode == OGLK_W || keycode == XK_W)
		plane->position = ft_fvector3_sum(plane->position, camera->normal);
	else if (keycode == OGLK_SPACE || keycode == XK_SPACE)
		plane->position.y += 1.0f;
	else if (keycode == OGLK_SHIFT || keycode == XK_SHIFT)
		plane->position.y -= 1.0f;
	else
		on_press_key_rotate(plane, keycode);
}

static inline void	on_press_key_rotate(t_plane *plane, int keycode)
{
	if (keycode == OGLK_ARROW_LEFT || keycode == XK_ARROW_LEFT)
		rotate_plane_yaw(plane, 0.1f);
	else if (keycode == OGLK_ARROW_RIGHT || keycode == XK_ARROW_RIGHT)
		rotate_plane_yaw(plane, -0.1f);
	else if (keycode == OGLK_ARROW_DOWN || keycode == XK_ARROW_DOWN)
		rotate_plane_pitch(plane, -0.1f);
	else if (keycode == OGLK_ARROW_UP || keycode == XK_ARROW_UP)
		rotate_plane_pitch(plane, 0.1f);
}

static inline void	rotate_plane_yaw(t_plane *plane, float theta)
{
	t_fvector3	global;

	global = (t_fvector3){0.0f, 1.0f, 0.0f};
	plane->normal = ft_fnormalize(rotate_object(plane->normal, global, theta));
	plane->right = ft_fnormalize(rotate_object(plane->right, global, theta));
	plane->up = ft_fnormalize(ft_fcross_product(plane->right, plane->normal));
}

static inline void	rotate_plane_pitch(t_plane *plane, float theta)
{
	t_fvector3	global;

	global = (t_fvector3){1.0f, 0.0f, 0.0f};
	plane->normal = ft_fnormalize(
			rotate_object(plane->normal, global, theta));
	plane->up = ft_fnormalize(rotate_object(plane->up, global, theta));
	plane->right = ft_fnormalize(ft_fcross_product(plane->normal, plane->up));
}
