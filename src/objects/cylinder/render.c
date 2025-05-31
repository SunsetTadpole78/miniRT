/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/31 21:10:28 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void		init_hit(t_ray *ray, t_hit_data *hit,
							t_cylinder *cylinder, float dist);
static inline t_rgb		get_base_color(t_cylinder *cy, t_fvector3 impact_point,
							t_pattern pattern);
static inline t_fvector3	get_cylinder_normal(int type,
							t_fvector3 impact_point, t_cylinder *cylinder);
static inline t_rgb		display_texture(t_texture texture, t_cylinder *cy,
							t_fvector3 diff,
							float h);
/* -------------------------------------------------------------------------- */

void	render_cylinder(t_minirt *mrt, t_ray *ray, t_object *object, int depth)
{
	float		dist;
	t_cylinder	*cylinder;
	t_hit_data	hit;
	int			inside;
	t_ray		reflect_ray;

	dist = intersect_cylinder(ray, object, 1.0f);
	if (dist < 0.0f || dist > ray->dist)
		return ;
	cylinder = (t_cylinder *)object;
	init_hit(ray, &hit, cylinder, dist);
	inside = is_inside_cylinder(object, ray->origin);
	if (inside)
		hit.normal = ft_fvector3_scale(hit.normal, -1);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			get_base_color(cylinder, hit.impact_point, cylinder->pattern));
	reflect_ray = *ray;
	specular_reflection(&reflect_ray, &hit, cylinder->pattern.smoothness);
	ray->color = ft_rgb_lerp(ray->color, ray_tracer(mrt, &reflect_ray,
				depth + 1), cylinder->pattern.mattifying);
	if (cylinder->selected)
		apply_selection_effect(&ray->color);
	ray->dist = dist;
}

static inline void	init_hit(t_ray *ray, t_hit_data *hit, t_cylinder *cylinder,
	float dist)
{
	hit->object = (t_object *)cylinder;
	hit->impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, dist));
	hit->normal = get_cylinder_normal(ray->extra, hit->impact_point, cylinder);
	hit->position = cylinder->position;
}

static inline t_fvector3	get_cylinder_normal(int type,
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

static inline t_rgb	get_base_color(t_cylinder *cy, t_fvector3 impact_point,
	t_pattern pattern)
{
	t_fvector3	diff;
	t_fvector3	proj;
	float		h;
	float		angle;

	if (pattern.id != 'c' && cy->texture.image == NULL)
		return (pattern.main_color);
	diff = ft_fvector3_diff(impact_point, cy->position);
	h = ft_fdot_product(diff, cy->normal);
	if (cy->texture.image != NULL)
		return (display_texture(cy->texture, cy, diff, h));
	proj = ft_fvector3_diff(diff, ft_fvector3_scale(cy->normal, h));
	angle = atan2f(ft_fdot_product(proj, cy->up),
			ft_fdot_product(proj, cy->right));
	if (angle < 0.0f)
		angle += 2.0f * M_PI;
	if ((int)(floor(angle * 3.0f + EPSILON)
		+ floor((h + cy->half_height) * 0.3f + EPSILON)) % 2 == 0)
		return (pattern.secondary_color);
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_texture texture, t_cylinder *cy,
	t_fvector3 diff, float h)
{
	float		u;
	float		v;
	t_fvector3	proj;
	float		angle;

	if (fabsf(fabsf(h) - cy->half_height) < EPSILON)
	{
		u = ft_fdot_product(diff, cy->right) / (2.0f * cy->radius) + 0.5f;
		v = ft_fdot_product(diff, cy->up) / (2.0f * cy->radius) + 0.5f;
	}
	else
	{
		proj = ft_fvector3_diff(diff, ft_fvector3_scale(cy->normal, h));
		angle = atan2f(ft_fdot_product(proj, cy->up),
				ft_fdot_product(proj, cy->right));
		if (angle < 0.0f)
			angle += 2.0f * M_PI;
		u = (angle / (2.0f * M_PI)) * 2.5f;
		v = ((h + cy->half_height) / (2.0f * cy->half_height));
	}
	u -= floorf(u);
	v -= floorf(v);
	return (texture_pixel_to_rgb(&texture,
			(int)(u * texture.width) % texture.width,
		(int)(v * texture.height) % texture.height));
}
