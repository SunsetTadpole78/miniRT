/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/26 19:46:34 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb	get_base_color(t_cylinder *cy, t_pattern pattern,
						t_fvector3 impact_point);
static inline t_rgb	display_texture(t_mlx_image texture, t_cylinder *cy,
						t_fvector3 diff,
						float h);
/* -------------------------------------------------------------------------- */

void	apply_lights_cylinder(t_minirt *mrt, t_ray *ray, t_object *object,
		int depth)
{
	t_cylinder	*cylinder;
	t_hit_data	hit;
	int			inside;
	t_rgb		base;
	t_ray		reflect_ray;

	cylinder = (t_cylinder *)object;
	inside = init_cylinder(ray, &hit, cylinder);
	base = get_base_color(cylinder, cylinder->pattern, hit.impact_point);
	if (base.r != 0 || base.g != 0 || base.b != 0)
		ray->color = apply_lights_modifier(
				get_lights_modifier(mrt, &hit, inside), base);
	if (!inside && cylinder->pattern.mattifying != 0.0f && hit.level != 0.0f)
	{
		reflect_ray = *ray;
		specular_reflection(&reflect_ray, &hit,
			cylinder->pattern.smoothness_factor);
		ray->color = ft_rgb_lerp(ray->color, ray_tracer(mrt, &reflect_ray,
					depth + 1), cylinder->pattern.mattifying);
	}
	if (cylinder->selected)
		apply_selection_effect(&ray->color);
}

static inline t_rgb	get_base_color(t_cylinder *cy, t_pattern pattern,
	t_fvector3 impact_point)
{
	t_fvector3	diff;
	t_fvector3	proj;
	float		h;
	float		angle;

	if (pattern.id != 'c' && !pattern.path)
		return (pattern.main_color);
	diff = ft_fvector3_diff(impact_point, cy->position);
	h = diff.x * cy->normal.x + diff.y * cy->normal.y + diff.z * cy->normal.z;
	if (pattern.id == 'c')
	{
		proj = ft_fvector3_diff(diff, ft_fvector3_scale(cy->normal, h));
		angle = atan2f(proj.x * cy->up.x + proj.y * cy->up.y + proj.z
				* cy->up.z, proj.x * cy->right.x + proj.y
				* cy->right.y + proj.z * cy->right.z);
		if (angle < 0.0f)
			angle += 2.0f * M_PI;
		if ((int)(floorf(angle * 3.0f + EPSILON)
			+ floorf((h + cy->half_height) * 0.3f + EPSILON)) & 1)
			return (pattern.secondary_color);
	}
	if (pattern.path)
		return (display_texture(pattern.texture, cy, diff, h));
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_mlx_image texture, t_cylinder *cy,
	t_fvector3 diff, float h)
{
	float		u;
	float		v;
	t_fvector3	proj;
	float		angle;

	if (fabsf(fabsf(h) - cy->half_height) < EPSILON)
	{
		u = (diff.x * cy->right.x + diff.y * cy->right.y + diff.z * cy->right.z)
			/ cy->diameter + 0.5f;
		v = (diff.x * cy->up.x + diff.y * cy->up.y + diff.z * cy->up.z)
			/ cy->diameter + 0.5f;
	}
	else
	{
		proj = ft_fvector3_diff(diff, ft_fvector3_scale(cy->normal, h));
		angle = atan2f(proj.x * cy->up.x + proj.y * cy->up.y + proj.z
				* cy->up.z, proj.x * cy->right.x + proj.y
				* cy->right.y + proj.z * cy->right.z);
		u = (angle + ((angle < 0.0f) * 2.0f * M_PI)) / (2.0f * M_PI);
		v = (h + cy->half_height) / cy->height;
	}
	return (mlx_pixel_to_rgb(texture,
			(int)((u - floorf(u)) * texture.width) % texture.width,
		(int)((v - floorf(v)) * texture.height) % texture.height));
}

/*
static inline t_fvector3	bump_mapping(t_mlx_image bump, t_cylinder *cy,
	t_fvector3 diff, float h, t_fvector3 normal)
{
	t_rgb	c;
	float	intensity;
	float	offset;

	c = display_texture(bump, cy, diff, h);
	intensity = (c.r + c.g + c.b) / (3.0f * 255.0f);
	offset = 0.8f * (intensity - 0.5f);
	return (ft_fnormalize(ft_fvector3_sum(normal,
			(t_fvector3){offset, offset, offset})));
}
*/
