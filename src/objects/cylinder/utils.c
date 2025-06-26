/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:34:10 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 16:21:15 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_fvector3	get_normal(int type, t_fvector3 impact_point,
								t_cylinder *cylinder);
/* -------------------------------------------------------------------------- */

int	init_cylinder(t_ray *ray, t_hit_data *hit, t_cylinder *cylinder)
{
	int	inside;

	hit->object = (t_object *)cylinder;
	hit->impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, ray->dist));
	hit->normal = get_normal(ray->extra, hit->impact_point, cylinder);
	hit->position = cylinder->position;
	hit->diff = ft_fvector3_diff(hit->impact_point, hit->position);
	hit->proj = ft_fvector3_diff(hit->diff,
			ft_fvector3_scale(cylinder->normal, hit->h));
	hit->u = 0.0f;
	hit->v = 0.0f;
	hit->h = ft_fdot_product(hit->diff, cylinder->normal);
	inside = is_inside_cylinder((t_object *)cylinder, ray->origin);
	if (inside)
		hit->normal = ft_fvector3_scale(hit->normal, -1);
	return (inside);
}

static inline t_fvector3	get_normal(int type,
	t_fvector3 impact_point, t_cylinder *cylinder)
{
	if (type == 1)
		return (ft_fvector3_scale(cylinder->normal, -1.0f));
	if (type == 2)
		return (cylinder->normal);
	return (ft_fnormalize(ft_fvector3_diff(impact_point,
				ft_fvector3_sum(cylinder->position,
					ft_fvector3_scale(cylinder->normal,
						ft_fdot_product(ft_fvector3_diff(
								impact_point,
								cylinder->position),
							cylinder->normal))))));
}

int	is_inside_cylinder(t_object *object, t_fvector3 point)
{
	t_cylinder	*cylinder;
	t_fvector3	diff;
	float		projection;
	t_fvector3	radial;

	cylinder = (t_cylinder *)object;
	diff = ft_fvector3_diff(point, cylinder->position);
	projection = ft_fdot_product(diff, cylinder->normal);
	if (!cylinder->infinite && (fabsf(projection) > cylinder->half_height))
		return (0);
	radial = (t_fvector3){
		diff.x - projection * cylinder->normal.x,
		diff.y - projection * cylinder->normal.y,
		diff.z - projection * cylinder->normal.z};
	if (ft_fdot_product(radial, radial) < cylinder->radius * cylinder->radius)
		return (1);
	return (0);
}

t_object	*duplicate_cylinder(t_object *object)
{
	t_cylinder	*cylinder;
	t_cylinder	*new;

	cylinder = (t_cylinder *)object;
	new = malloc(sizeof(t_cylinder));
	if (!new)
		return (NULL);
	new->id = CYLINDER_ID;
	new->position = cylinder->position;
	new->normal = cylinder->normal;
	new->right = cylinder->right;
	new->up = cylinder->up;
	new->pattern = cylinder->pattern;
	new->diameter = cylinder->diameter;
	new->radius = cylinder->radius;
	new->height = cylinder->height;
	new->half_height = cylinder->half_height;
	new->infinite = cylinder->infinite;
	new->methods = cylinder->methods;
	new->selected = 0;
	return ((t_object *)new);
}
