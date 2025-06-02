/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:08:17 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 11:31:30 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void		update_size(t_sphere *sphere, float incrementation);
static inline t_fvector3	sum_inversed(t_fvector3 position,
							t_fvector3 normal);
/* -------------------------------------------------------------------------- */

void	on_press_key_sphere(t_object *object, int keycode, t_camera *camera)
{
	t_sphere	*sphere;

	sphere = (t_sphere *)object;
	if (keycode == OGLK_A || keycode == XK_A)
		sphere->position = ft_fvector3_sum(sphere->position, camera->right);
	else if (keycode == OGLK_S || keycode == XK_S)
		sphere->position = sum_inversed(sphere->position, camera->normal);
	else if (keycode == OGLK_D || keycode == XK_D)
		sphere->position = sum_inversed(sphere->position, camera->right);
	else if (keycode == OGLK_W || keycode == XK_W)
		sphere->position = ft_fvector3_sum(sphere->position, camera->normal);
	else if (keycode == OGLK_EQUAL || keycode == XK_EQUAL)
		update_size(sphere, 1.0f);
	else if (keycode == OGLK_MINUS || keycode == XK_MINUS)
		update_size(sphere, -1.0f);
	else if (keycode == OGLK_SPACE || keycode == XK_SPACE)
		sphere->position.y += 1.0f;
	else if (keycode == OGLK_SHIFT || keycode == XK_SHIFT)
		sphere->position.y -= 1.0f;
}

static inline void	update_size(t_sphere *sphere, float incrementation)
{
	if (sphere->diameter + incrementation <= 0)
		return ;
	sphere->diameter += incrementation;
	sphere->radius += incrementation * 0.5f;
}

static inline t_fvector3	sum_inversed(t_fvector3 position, t_fvector3 normal)
{
	return (ft_fvector3_sum(position, ft_fvector3_scale(normal, -1.0f)));
}
