/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:08:17 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/30 15:36:00 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void		update_level(t_light *light, float incrementation);
static inline t_fvector3	sum_inversed(t_fvector3 position,
							t_fvector3 normal);
/* -------------------------------------------------------------------------- */

void	on_press_key_light(t_object *object, int keycode, t_camera *camera)
{
	t_light	*light;

	light = (t_light *)object;
	if (keycode == OGLK_A || keycode == XK_A)
		light->position = ft_fvector3_sum(light->position, camera->right);
	else if (keycode == OGLK_S || keycode == XK_S)
		light->position = sum_inversed(light->position, camera->normal);
	else if (keycode == OGLK_D || keycode == XK_D)
		light->position = sum_inversed(light->position, camera->right);
	else if (keycode == OGLK_W || keycode == XK_W)
		light->position = ft_fvector3_sum(light->position, camera->normal);
	else if (keycode == OGLK_EQUAL || keycode == XK_EQUAL)
		update_level(light, 0.1f);
	else if (keycode == OGLK_MINUS || keycode == XK_MINUS)
		update_level(light, -0.1f);
	else if (keycode == OGLK_SPACE || keycode == XK_SPACE)
		light->position.y += 1.0f;
	else if (keycode == OGLK_SHIFT || keycode == XK_SHIFT)
		light->position.y -= 1.0f;
}

static inline void	update_level(t_light *light, float incrementation)
{
	if (light->scale + incrementation <= 0)
		return ;
	light->level += incrementation;
	light->radius = light->level * light->scale;
}

static inline t_fvector3	sum_inversed(t_fvector3 position, t_fvector3 normal)
{
	return (ft_fvector3_sum(position, ft_fvector3_scale(normal, -1.0f)));
}
