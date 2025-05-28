/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:34:10 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/29 00:28:59 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	is_inside_cylinder(t_hit_data hit, t_fvector3 point)
{
	t_cylinder	*cylinder;
	t_fvector3	diff;
	float		projection;
	t_fvector3	radial;

	cylinder = (t_cylinder *)hit.object;
	diff = ft_fvector3_diff(point, cylinder->position);
	projection = ft_fdot_product(diff, cylinder->normal);
	if (fabsf(projection) > cylinder->half_height)
		return (0);
	radial = (t_fvector3){
		diff.x - projection * cylinder->normal.x,
		diff.y - projection * cylinder->normal.y,
		diff.z - projection * cylinder->normal.z};
	if (ft_fdot_product(radial, radial) <= cylinder->radius * cylinder->radius)
		return (1);
	return (0);
}
