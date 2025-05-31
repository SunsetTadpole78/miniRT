/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 17:57:23 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/31 17:57:29 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void	update_size(t_cone *cone, float incrementation);
static inline void	on_press_key_rotate(t_cone *cone, int keycode);
static inline void	rotate_cone_yaw(t_cone *cone, float theta);
static inline void	rotate_cone_pitch(t_cone *cone,
						float theta);
/* -------------------------------------------------------------------------- */

void	on_press_key_cone(t_object *object, int keycode, t_camera *camera)
{
	t_cone	*cone;

	cone = (t_cone *)object;
	if (keycode == OGLK_A || keycode == XK_A)
		cone->position = ft_fvector3_sum(cone->position, camera->right);
	else if (keycode == OGLK_S || keycode == XK_S)
		cone->position = ft_fvector3_sum(cone->position,
				ft_fvector3_scale(camera->normal, -1.0f));
	else if (keycode == OGLK_D || keycode == XK_D)
		cone->position = ft_fvector3_sum(cone->position,
				ft_fvector3_scale(camera->right, -1.0f));
	else if (keycode == OGLK_W || keycode == XK_W)
		cone->position = ft_fvector3_sum(cone->position,
				camera->normal);
	else if (keycode == OGLK_EQUAL || keycode == XK_EQUAL)
		update_size(cone, 1.0f);
	else if (keycode == OGLK_MINUS || keycode == XK_MINUS)
		update_size(cone, -1.0f);
	else if (keycode == OGLK_SPACE || keycode == XK_SPACE)
		cone->position.y += 1.0f;
	else if (keycode == OGLK_SHIFT || keycode == XK_SHIFT)
		cone->position.y -= 1.0f;
	else
		on_press_key_rotate(cone, keycode);
}

static inline void	on_press_key_rotate(t_cone *cone, int keycode)
{
	if (keycode == OGLK_ARROW_LEFT || keycode == XK_ARROW_LEFT)
		rotate_cone_yaw(cone, 0.2f);
	else if (keycode == OGLK_ARROW_RIGHT || keycode == XK_ARROW_RIGHT)
		rotate_cone_yaw(cone, -0.2f);
	else if (keycode == OGLK_ARROW_DOWN || keycode == XK_ARROW_DOWN)
		rotate_cone_pitch(cone, 0.2f);
	else if (keycode == OGLK_ARROW_UP || keycode == XK_ARROW_UP)
		rotate_cone_pitch(cone, -0.2f);
}

static inline void	update_size(t_cone *cone, float incrementation)
{
	float	diameter;
	float	scale_factor;

	diameter = cone->diameter;
	if (diameter + incrementation <= 0.0f)
		return ;
	scale_factor = (diameter + incrementation) / diameter;
	cone->diameter *= scale_factor;
	cone->radius *= scale_factor;
	cone->height *= scale_factor;
	cone->half_height *= scale_factor;
}

static inline void	rotate_cone_yaw(t_cone *cone, float theta)
{
	t_fvector3	global;

	global = (t_fvector3){0.0f, 1.0f, 0.0f};
	cone->normal = ft_fnormalize(
			rotate_object(cone->normal, global, theta));
	cone->right = ft_fnormalize(
			rotate_object(cone->right, global, theta));
	cone->up = ft_fnormalize(
			ft_fcross_product(cone->right, cone->normal));
}

static inline void	rotate_cone_pitch(t_cone *cone, float theta)
{
	cone->normal = ft_fnormalize(
			rotate_object(cone->normal, cone->right, theta));
	cone->up = ft_fnormalize(
			rotate_object(cone->up, cone->right, theta));
	cone->right = ft_fnormalize(ft_fvector3_scale(
				ft_fcross_product(cone->up, cone->normal), -1.0f));
}
