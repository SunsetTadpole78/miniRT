/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:18:09 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/29 15:01:36 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void	update_size(t_cylinder *cylinder, float incrementation);
static inline void	on_press_key_rotate(t_cylinder *cylinder, int keycode);
static inline void	rotate_cylinder_yaw(t_cylinder *cylinder, float theta);
static inline void	rotate_cylinder_pitch(t_cylinder *cylinder,
						float theta);
/* -------------------------------------------------------------------------- */

void	on_press_key_cylinder(t_object *object, int keycode, t_camera *camera)
{
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *)object;
	if (keycode == OGLK_A || keycode == XK_A)
		cylinder->position = ft_fvector3_sum(cylinder->position, camera->right);
	else if (keycode == OGLK_S || keycode == XK_S)
		cylinder->position = ft_fvector3_sum(cylinder->position,
				ft_fvector3_scale(camera->normal, -1.0f));
	else if (keycode == OGLK_D || keycode == XK_D)
		cylinder->position = ft_fvector3_sum(cylinder->position,
				ft_fvector3_scale(camera->right, -1.0f));
	else if (keycode == OGLK_W || keycode == XK_W)
		cylinder->position = ft_fvector3_sum(cylinder->position,
				camera->normal);
	else if (keycode == OGLK_EQUAL || keycode == XK_EQUAL)
		update_size(cylinder, 1.0f);
	else if (keycode == OGLK_MINUS || keycode == XK_MINUS)
		update_size(cylinder, -1.0f);
	else if (keycode == OGLK_SPACE || keycode == XK_SPACE)
		cylinder->position.y += 1.0f;
	else if (keycode == OGLK_SHIFT || keycode == XK_SHIFT)
		cylinder->position.y -= 1.0f;
	else
		on_press_key_rotate(cylinder, keycode);
}

static inline void	on_press_key_rotate(t_cylinder *cylinder, int keycode)
{
	if (keycode == OGLK_ARROW_LEFT || keycode == XK_ARROW_LEFT)
		rotate_cylinder_yaw(cylinder, 0.2f);
	else if (keycode == OGLK_ARROW_RIGHT || keycode == XK_ARROW_RIGHT)
		rotate_cylinder_yaw(cylinder, -0.2f);
	else if (keycode == OGLK_ARROW_DOWN || keycode == XK_ARROW_DOWN)
		rotate_cylinder_pitch(cylinder, 0.2f);
	else if (keycode == OGLK_ARROW_UP || keycode == XK_ARROW_UP)
		rotate_cylinder_pitch(cylinder, -0.2f);
}

static inline void	update_size(t_cylinder *cylinder, float incrementation)
{
	float	diameter;
	float	scale_factor;

	diameter = cylinder->diameter;
	if (diameter + incrementation <= 0.0f)
		return ;
	scale_factor = (diameter + incrementation) / diameter;
	cylinder->diameter *= scale_factor;
	cylinder->radius *= scale_factor;
	cylinder->height *= scale_factor;
	cylinder->half_height *= scale_factor;
}

static inline void	rotate_cylinder_yaw(t_cylinder *cylinder, float theta)
{
	t_fvector3	global;

	global = (t_fvector3){0.0f, 1.0f, 0.0f};
	cylinder->normal = ft_fnormalize(
			rotate_object(cylinder->normal, global, theta));
	cylinder->right = ft_fnormalize(
			rotate_object(cylinder->right, global, theta));
	cylinder->up = ft_fnormalize(
			ft_fcross_product(cylinder->right, cylinder->normal));
}

static inline void	rotate_cylinder_pitch(t_cylinder *cylinder, float theta)
{
	t_fvector3	new_right;

	cylinder->normal = ft_fnormalize(
			rotate_object(cylinder->normal, cylinder->right, theta));
	cylinder->up = ft_fnormalize(
			rotate_object(cylinder->up, cylinder->right, theta));
	new_right = ft_fnormalize(
			ft_fcross_product(cylinder->up, cylinder->normal));
	if (ft_fdot_product(cylinder->right, new_right) < 0.0f)
		new_right = ft_fnormalize(ft_fvector3_scale(new_right, -1.0f));
	cylinder->right = new_right;
}
