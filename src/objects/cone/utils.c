/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 12:46:20 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/31 21:54:17 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_fvector3	get_cone_normal(int type, t_fvector3 impact_point,
								t_cone *cone);
/* -------------------------------------------------------------------------- */

void	init_cone_hit(t_ray *ray, t_hit_data *hit, t_cone *cone, float dist)
{
	hit->object = (t_object *)cone;
	hit->impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, dist));
	hit->normal = get_cone_normal(ray->extra, hit->impact_point, cone);
	hit->position = cone->position;
}

static inline t_fvector3	get_cone_normal(int type, t_fvector3 impact_point,
		t_cone *cone)
{
	t_fvector3	apex_to_p;

	if (type == 1)
		return (cone->normal);
	apex_to_p = ft_fvector3_diff(impact_point, cone->position);
	return (ft_fnormalize(ft_fvector3_diff(apex_to_p, ft_fvector3_scale(
					ft_fvector3_scale(cone->normal, ft_fdot_product(apex_to_p,
							cone->normal)), 1 + cone->k2))));
}

int	is_inside_init_cone(t_object *object, t_fvector3 *origin,
	t_fvector3 *normal)
{
	int	inside;

	inside = is_inside_cone(object, *origin);
	if (inside)
		*normal = ft_fvector3_scale(*normal, -1);
	return (inside);
}

int	is_inside_cone(t_object *object, t_fvector3 point)
{
	t_cone		*cone;
	t_fvector3	local_point;
	t_fvector3	oc;
	float		radius_at_y;

	cone = (t_cone *)object;
	oc = ft_fvector3_diff(point, cone->position);
	local_point = (t_fvector3){
		ft_fdot_product(oc, cone->right),
		ft_fdot_product(oc, ft_fnormalize(cone->normal)),
		ft_fdot_product(oc, cone->up)
	};
	if (!cone->infinite && (local_point.y < 0.0f
			|| local_point.y > cone->height))
		return (0);
	radius_at_y = local_point.y * cone->k;
	if (ft_fhorizontal_magnitude(local_point) <= radius_at_y * radius_at_y)
		return (1);
	return (0);
}
