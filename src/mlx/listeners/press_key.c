/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   press_key.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/30 14:52:36 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

int	on_press_key(int keycode, t_minirt *mrt)
{
	void	(*on_press_key)(t_object *, int, t_camera *);
	t_mlx	*mlx;

	if (keycode == OGLK_ESC || keycode == XK_ESC)
		close_window(mrt);
	else if (keycode == OGLK_R || keycode == XK_R)
	{
		mlx = mrt->mlx;
		if (mlx->update == 2)
			mlx->update = 1;
		else
			mlx->update = 2;
		return (0);
	}
	else if (!mrt->selected)
		on_press_key_camera(mrt->camera, keycode);
	else
	{
		on_press_key = mrt->selected->methods->on_press_key;
		if (on_press_key)
			on_press_key(mrt->selected, keycode, mrt->camera);
	}
	mrt->mlx->count = 0;
	mrt->mlx->update = 1;
	return (0);
}
