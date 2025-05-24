/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caps.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 02:56:27 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/24 16:26:04 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

float	intersect_cap(t_fvector3 local_origin, t_fvector3 local_dir,
	float radius, float half_height)
{
	float	t;
	float	x;
	float	z;

	t = (half_height - local_origin.y) / local_dir.y;
	if (t < 0.001f)
		return (-1);
	x = local_origin.x + t * local_dir.x;
	z = local_origin.z + t * local_dir.z;
	if ((x * x + z * z) <= (radius * radius))
		return (t);
	return (-1);
}
