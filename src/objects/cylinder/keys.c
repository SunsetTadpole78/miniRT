/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:18:09 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/28 18:36:01 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void		update_size(t_cylinder *cylinder, float incrementation);
static inline t_fvector3	sum_inversed(t_fvector3 position,
							t_fvector3 normal);
/* -------------------------------------------------------------------------- */

void	on_press_key_cylinder(t_object *object, int keycode, t_camera *camera)
{
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *)object;
	if (keycode == OGLK_A || keycode == XK_A)
		cylinder->position = ft_fvector3_sum(cylinder->position, camera->right);
	else if (keycode == OGLK_S || keycode == XK_S)
		cylinder->position = sum_inversed(cylinder->position, camera->normal);
	else if (keycode == OGLK_D || keycode == XK_D)
		cylinder->position = sum_inversed(cylinder->position, camera->right);
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
}

static inline void	update_size(t_cylinder *cylinder, float incrementation)
{
	float	diameter;
	float	scale_factor;

	if (cylinder->diameter + incrementation <= 0)
		return ;
	diameter = cylinder->diameter;
	scale_factor = (diameter + incrementation) / diameter;
	cylinder->diameter *= scale_factor;
	cylinder->radius *= scale_factor;
	cylinder->height *= scale_factor;
	cylinder->half_height *= scale_factor;
}

static inline t_fvector3	sum_inversed(t_fvector3 position, t_fvector3 normal)
{
	return (ft_fvector3_sum(position, ft_fvector3_scale(normal, -1.0f)));
}
