/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_fvector3	get_normal(int type, t_fvector3 impact_point,
								t_cylinder *cylinder);
static inline t_rgb		get_base_color(t_cylinder *cy, t_fvector3 impact_point,
								t_pattern pattern);
/* -------------------------------------------------------------------------- */

void	render_cylinder(t_minirt *mrt, t_ray *ray, t_object *object, int depth)
{
	float		dist;
	t_cylinder	*cylinder;
	t_hit_data	hit;
	int			inside;

	(void)depth;
	cylinder = (t_cylinder *)object;
	dist = intersect_cylinder(ray, object, 1.0f);
	if (dist < 0.0f || dist > ray->dist)
		return ;
	hit.object = object;
	hit.position = cylinder->position;
	hit.impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, dist));
	hit.normal = get_normal(ray->extra, hit.impact_point, cylinder);
	inside = is_inside_cylinder(object, ray->origin);
	if (inside)
		hit.normal = ft_fvector3_scale(hit.normal, -1);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			get_base_color(cylinder, hit.impact_point, cylinder->pattern));
	ray->dist = dist;
}

float	intersect_cylinder(t_ray *ray, t_object *object, float amplifier)
{
	t_cylinder	*cylinder;
	t_fvector3	local_origin;
	t_fvector3	local_dir;
	t_fvector3	t;
	t_fvector2	amplified;

	cylinder = (t_cylinder *)object;
	normalize_side(&local_origin, &local_dir, *ray, cylinder);
	t.z = apply_side_equation(local_origin, local_dir, cylinder, amplifier);
	if (fabsf(local_dir.y) < 1e-6f)
		return (t.z);
	amplified = (t_fvector2){cylinder->radius * amplifier,
		cylinder->half_height * amplifier};
	t.x = intersect_cap(local_origin, local_dir, amplified.x, -amplified.y);
	t.y = intersect_cap(local_origin, local_dir, amplified.x, amplified.y);
	ray->extra = t.x > EPSILON && (t.z < 0 || t.x < t.z);
	if (ray->extra == 1)
		t.z = t.x;
	if (t.y > EPSILON && (t.z < 0 || t.y < t.z))
	{
		ray->extra = 2;
		t.z = t.y;
	}
	return (t.z);
}

static inline t_fvector3	get_normal(int type, t_fvector3 impact_point,
		t_cylinder *cylinder)
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

static inline t_rgb	get_base_color(t_cylinder *cy, t_fvector3 impact_point,
	t_pattern pattern)
{
	t_fvector3	diff;
	t_fvector3	proj;
	float		h;
	float		angle;

	if (pattern.id != 'c')
		return (pattern.main_color);
	diff = ft_fvector3_diff(impact_point, cy->position);
	h = ft_fdot_product(diff, cy->normal);
	proj = ft_fvector3_diff(diff, ft_fvector3_scale(cy->normal, h));
	angle = atan2f(ft_fdot_product(proj, cy->up),
			ft_fdot_product(proj, cy->right));
	if (angle < 0)
		angle += 2.0f * M_PI;
	if ((int)(floor(angle * 3.0f + EPSILON)
		+ floor((h + cy->half_height) * 0.3f + EPSILON)) % 2 == 0)
		return (pattern.secondary_color);
	return (pattern.main_color);
}
