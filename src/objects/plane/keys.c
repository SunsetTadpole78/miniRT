/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:08:17 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/29 16:58:33 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_fvector3	sum_inversed(t_fvector3 position,
								t_fvector3 normal);
static inline void	rotate_x(t_plane *plane, float theta);
static inline void	rotate_y(t_plane *plane, float theta);
static inline void	rotate_z(t_plane *plane, float theta);
/* -------------------------------------------------------------------------- */

void	on_press_key_plane(t_object *object, int keycode, t_camera *camera)
{
	t_plane	*plane;

	plane = (t_plane *)object;
	if (keycode == OGLK_A || keycode == XK_A)
		plane->position = ft_fvector3_sum(plane->position, camera->right);
	else if (keycode == OGLK_S || keycode == XK_S)
		plane->position = sum_inversed(plane->position, camera->normal);
	else if (keycode == OGLK_D || keycode == XK_D)
		plane->position = sum_inversed(plane->position, camera->right);
	else if (keycode == OGLK_W || keycode == XK_W)
		plane->position = ft_fvector3_sum(plane->position, camera->normal);
	else if (keycode == OGLK_SPACE || keycode == XK_SPACE)
		plane->position.y += 1.0f;
	else if (keycode == OGLK_SHIFT || keycode == XK_SHIFT)
		plane->position.y -= 1.0f;
	else if (keycode == OGLK_U || keycode == XK_U)
		rotate_x(plane, -0.1f);
	else if (keycode == OGLK_J || keycode == XK_J)
		rotate_x(plane, 0.1f);
	else if (keycode == OGLK_I || keycode == XK_I)
		rotate_y(plane, -0.1f);
	else if (keycode == OGLK_K || keycode == XK_K)
		rotate_y(plane, 0.1f);
	else if (keycode == OGLK_O || keycode == XK_O)
		rotate_z(plane, -0.1f);
	else if (keycode == OGLK_L || keycode == XK_L)
		rotate_z(plane, 0.1f);
}

static inline t_fvector3	sum_inversed(t_fvector3 position, t_fvector3 normal)
{
	return (ft_fvector3_sum(position, ft_fvector3_scale(normal, -1.0f)));
}

static inline void	rotate_x(t_plane *plane, float theta)
{
	float		cos_t;
	float		sin_t;
	t_fvector3	normal;
	t_fvector3	up;

	cos_t = cosf(theta);
	sin_t = sinf(theta);
	normal = plane->normal;
	up = plane->up;
	plane->normal = ft_fnormalize((t_fvector3){
			normal.x * cos_t + up.x * sin_t,
			normal.y * cos_t + up.y * sin_t,
			normal.z * cos_t + up.z * sin_t
			});
	plane->up = ft_fnormalize(ft_fcross_product(plane->right,
				plane->normal));
}

static inline void	rotate_y(t_plane *plane, float theta)
{
	float		cos_t;
	float		sin_t;
	t_fvector3	world;
	t_fvector3	normal;

	world = (t_fvector3){0, 1, 0};
	if (ft_fdot_product(plane->up, world) < 0)
	{
		theta = -theta;
		world.y = -1;
	}
	cos_t = cosf(theta);
	sin_t = sinf(theta);
	normal = plane->normal;
	plane->normal = ft_fnormalize((t_fvector3){
			normal.x * cos_t - normal.z * sin_t,
			normal.y,
			normal.x * sin_t + normal.z * cos_t
			});
	plane->right = ft_fnormalize(ft_fcross_product(plane->normal, world));
	plane->up = ft_fcross_product(plane->right, plane->normal);
}

static inline void	rotate_z(t_plane *plane, float theta)
{
	float	cos_t;
	float	sin_t;
	t_fvector3	normal;

	cos_t = cosf(theta);
	sin_t = sinf(theta);
	normal = plane->normal;
	plane->normal = ft_fnormalize((t_fvector3){
			normal.x * cos_t - normal.y * sin_t,
			normal.x * sin_t + normal.y * cos_t,
			normal.z
			});

	plane->right = ft_fnormalize(ft_fcross_product(plane->normal, (t_fvector3){0, 0, 1}));
	plane->up = ft_fcross_product(plane->right, plane->normal);
}
