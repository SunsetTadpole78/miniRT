/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release_key.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:11:37 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 12:38:28 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "keys.h"

int	on_release_key(int keycode, t_minirt *mrt)
{
	if (keycode == OGLK_LEFT_COMMAND || keycode == OGLK_RIGHT_COMMAND
		|| keycode == XK_LEFT_CONTROL || keycode == XK_RIGHT_CONTROL)
	{
		mrt->ctrl_pressed--;
		if (mrt->ctrl_pressed < 0)
			mrt->ctrl_pressed = 0;
	}
	return (1);
}
