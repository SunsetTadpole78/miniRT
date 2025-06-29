/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:34:10 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/30 01:52:02 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_fvector3	get_normal(int type, t_fvector3 impact_point,
								t_cylinder *cylinder);
static inline void		init_uv(t_hit_data *hit, t_cylinder *cylinder,
								int is_side);
/* -------------------------------------------------------------------------- */

int	init_cylinder(t_ray *ray, t_hit_data *hit, t_cylinder *cylinder)
{
	int	inside;

	hit->object = (t_object *)cylinder;
	hit->impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, ray->dist));
	hit->normal = get_normal(ray->extra, hit->impact_point, cylinder);
	hit->position = cylinder->position;
	inside = is_inside_cylinder((t_object *)cylinder, ray->origin);
	if (inside)
		hit->normal = ft_fvector3_scale(hit->normal, -1);
	hit->level = cylinder->default_level;
	hit->diff = ft_fvector3_diff(hit->impact_point, cylinder->position);
	hit->h = ft_fdot_product(hit->diff, cylinder->normal);
	hit->proj = ft_fvector3_diff(hit->diff, ft_fvector3_scale(cylinder->normal,
				hit->h));
	init_uv(hit, cylinder, ray->extra == 0);
	return (inside);
}

static inline void	init_uv(t_hit_data *hit, t_cylinder *cylinder, int is_side)
{
	float	angle;

	if (is_side)
	{
		angle = atan2f(hit->proj.x * cylinder->up.x + hit->proj.y
				* cylinder->up.y + hit->proj.z * cylinder->up.z, hit->proj.x
				* cylinder->right.x + hit->proj.y * cylinder->right.y
				+ hit->proj.z * cylinder->right.z);
		hit->u = (angle + (angle < 0.0f) * 2.0f * M_PI) / (2.0f * M_PI);
		hit->v = (hit->h + cylinder->half_height) / cylinder->height;
		return ;
	}
	hit->u = (hit->diff.x * cylinder->right.x + hit->diff.y
			* cylinder->right.y + hit->diff.z * cylinder->right.z)
		/ cylinder->diameter + 0.5f;
	hit->v = (hit->diff.x * cylinder->up.x + hit->diff.y * cylinder->up.y
			+ hit->diff.z * cylinder->up.z) / cylinder->diameter + 0.5f;
}

static inline t_fvector3	get_normal(int type,
	t_fvector3 impact_point, t_cylinder *cylinder)
{
	t_fvector3	diff;
	float		scale;

	if (type == 1)
	{
		return ((t_fvector3){
			cylinder->normal.x * -1.0f,
			cylinder->normal.y * -1.0f,
			cylinder->normal.z * -1.0f
		});
	}
	if (type == 2)
		return (cylinder->normal);
	diff = ft_fvector3_diff(impact_point, cylinder->position);
	scale = diff.x * cylinder->normal.x + diff.y * cylinder->normal.y
		+ diff.z * cylinder->normal.z;
	return (ft_fnormalize((t_fvector3){
			impact_point.x - (cylinder->position.x
				+ (cylinder->normal.x * scale)),
			impact_point.y - (cylinder->position.y
				+ (cylinder->normal.y * scale)),
			impact_point.z - (cylinder->position.z
				+ (cylinder->normal.z * scale))
		}));
}

int	is_inside_cylinder(t_object *object, t_fvector3 point)
{
	t_cylinder	*cylinder;
	t_fvector3	diff;
	float		projection;
	t_fvector3	radial;

	cylinder = (t_cylinder *)object;
	diff = ft_fvector3_diff(point, cylinder->position);
	projection = diff.x * cylinder->normal.x + diff.y * cylinder->normal.y
		+ diff.z * cylinder->normal.z;
	if (!cylinder->infinite && (fabsf(projection) > cylinder->half_height))
		return (0);
	radial = (t_fvector3){
		diff.x - projection * cylinder->normal.x,
		diff.y - projection * cylinder->normal.y,
		diff.z - projection * cylinder->normal.z};
	return ((radial.x * radial.x + radial.y * radial.y + radial.z * radial.z)
		<= cylinder->radius * cylinder->radius);
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
