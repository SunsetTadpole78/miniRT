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
static inline t_fvector3	sum_inversed(t_fvector3 position,
								t_fvector3 normal);
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
}

static inline t_fvector3	sum_inversed(t_fvector3 position, t_fvector3 normal)
{
	return (ft_fvector3_sum(position, ft_fvector3_scale(normal, -1.0f)));
}
