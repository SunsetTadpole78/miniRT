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
static inline void		on_press_key_rotate(t_plane *plane, int keycode);
static inline void		rotate_plane_yaw(t_plane *plane, float theta);
static inline void		rotate_plane_pitch(t_plane *plane, float theta);
static inline t_fvector3	rotate_vector(t_fvector3 v, t_fvector3 axis,
							float angle);
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
		rotate_plane_yaw(plane, 0.05f);
	else if (keycode == OGLK_ARROW_RIGHT || keycode == XK_ARROW_RIGHT)
		rotate_plane_yaw(plane, -0.05f);
	else if (keycode == OGLK_ARROW_DOWN || keycode == XK_ARROW_DOWN)
		rotate_plane_pitch(plane, -0.05f);
	else if (keycode == OGLK_ARROW_UP || keycode == XK_ARROW_UP)
		rotate_plane_pitch(plane, 0.05f);
}

static inline void	rotate_plane_yaw(t_plane *plane, float theta)
{
	t_fvector3		global;

	global = (t_fvector3){0.0f, 1.0f, 0.0f};
	plane->normal = ft_fnormalize(rotate_vector(plane->normal, global, theta));
	plane->right = ft_fnormalize(rotate_vector(plane->right, global, theta));
	plane->up = ft_fnormalize(ft_fcross_product(plane->right, plane->normal));
}

static inline void	rotate_plane_pitch(t_plane *plane, float theta)
{
	plane->normal = ft_fnormalize(
			rotate_vector(plane->normal, plane->right, theta));
	plane->up = ft_fnormalize(rotate_vector(plane->up, plane->right, theta));
	plane->right = ft_fnormalize(ft_fcross_product(plane->normal, plane->up));
}

static inline t_fvector3	rotate_vector(t_fvector3 v, t_fvector3 axis,
	float theta)
{
	t_fvector3	rotate;
	t_fvector3	cross;
	float		cos_t;
	float		sin_t;
	float		dot;

	cos_t = cosf(theta);
	sin_t = sinf(theta);
	dot = ft_fdot_product(axis, v);
	cross = ft_fcross_product(axis, v);
	rotate.x = v.x * cos_t + cross.x * sin_t + axis.x * dot * (1 - cos_t);
	rotate.y = v.y * cos_t + cross.y * sin_t + axis.y * dot * (1 - cos_t);
	rotate.z = v.z * cos_t + cross.z * sin_t + axis.z * dot * (1 - cos_t);
	return (rotate);
}
