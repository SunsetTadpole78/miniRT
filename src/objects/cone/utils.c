/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 12:46:20 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 16:17:56 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_fvector3	get_normal(int type, t_fvector3 impact_point,
								t_cone *cone);
/* -------------------------------------------------------------------------- */

int	init_cone(t_ray *ray, t_hit_data *hit, t_cone *cone)
{
	int	inside;

	hit->object = (t_object *)cone;
	hit->impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, ray->dist));
	hit->normal = get_normal(ray->extra, hit->impact_point, cone);
	hit->position = cone->position;
	hit->diff = (t_fvector3){0.0f, 0.0f, 0.0f};
	hit->proj = (t_fvector3){0.0f, 0.0f, 0.0f};
	hit->u = 0.0f;
	hit->v = 0.0f;
	hit->h = 0.0f;
	inside = is_inside_cone((t_object *)cone, ray->origin);
	if (inside)
		hit->normal = ft_fvector3_scale(hit->normal, -1);
	return (inside);
}

static inline t_fvector3	get_normal(int type, t_fvector3 impact_point,
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

t_object	*duplicate_cone(t_object *object)
{
	t_cone	*cone;
	t_cone	*new;

	cone = (t_cone *)object;
	new = malloc(sizeof(t_cone));
	if (!new)
		return (NULL);
	new->id = CONE_ID;
	new->position = cone->position;
	new->normal = cone->normal;
	new->right = cone->right;
	new->up = cone->up;
	new->pattern = cone->pattern;
	new->base_diameter = cone->base_diameter;
	new->infinite = cone->infinite;
	new->height = cone->height;
	new->k = cone->k;
	new->k2 = cone->k2;
	new->methods = cone->methods;
	new->selected = 0;
	return ((t_object *)new);
}
