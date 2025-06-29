/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/30 01:49:01 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb			get_base_color(t_cylinder *cy, t_pattern pattern,
								t_hit_data hit);
static inline t_rgb			display_texture(t_mlx_image texture, t_cylinder *cy,
								t_hit_data hit);
static inline t_fvector3		bump_mapping(t_cylinder *cy,
								t_mlx_image bump, t_hit_data hit, int is_side);
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
	if (cylinder->pattern.bump_path)
		hit.normal = bump_mapping(cylinder, cylinder->pattern.bump, hit,
				ray->extra == 0);
	base = get_base_color(cylinder, cylinder->pattern, hit);
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
	t_hit_data hit)
{
	float		angle;

	if (pattern.id != 'c' && !pattern.path)
		return (pattern.main_color);
	if (pattern.id == 'c')
	{
		hit.h = hit.diff.x * cy->normal.x + hit.diff.y
			* cy->normal.y + hit.diff.z * cy->normal.z;
		hit.proj = ft_fvector3_diff(hit.diff,
				ft_fvector3_scale(cy->normal, hit.h));
		angle = atan2f(hit.proj.x * cy->up.x + hit.proj.y * cy->up.y
				+ hit.proj.z * cy->up.z, hit.proj.x * cy->right.x
				+ hit.proj.y * cy->right.y + hit.proj.z * cy->right.z);
		if ((int)(floorf((angle + (angle < 0.0f) * 2.0f * M_PI)
				* 3.0f + EPSILON) + floorf((hit.h + cy->half_height)
				* 0.3f + EPSILON)) & 1)
			return (pattern.secondary_color);
	}
	if (pattern.path)
		return (display_texture(pattern.texture, cy, hit));
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_mlx_image texture, t_cylinder *cy,
	t_hit_data hit)
{
	if (fabsf(fabsf(hit.h) - cy->half_height) < EPSILON)
	{
		hit.u = (hit.diff.x * cy->right.x + hit.diff.y * cy->right.y
				+ hit.diff.z * cy->right.z) / cy->diameter + 0.5f;
		hit.v = (hit.diff.x * cy->up.x + hit.diff.y * cy->up.y
				+ hit.diff.z * cy->up.z) / cy->diameter + 0.5f;
	}
	return (mlx_pixel_to_rgb(texture,
			(int)((hit.u - floorf(hit.u)) * texture.width) % texture.width,
		(int)((hit.v - floorf(hit.v)) * texture.height) % texture.height));
}

static inline t_fvector3	bump_mapping(t_cylinder *cy,
		t_mlx_image bump, t_hit_data hit, int is_side)
{
	t_rgb		c;
	float		hc;
	float		du;
	float		dv;

	if (!is_side)
		return (cap_bump_mapping(cy, bump, hit));
	c = mlx_pixel_to_rgb(bump,
			(int)(hit.u * (float)bump.width) % bump.width,
			(int)(hit.v * (float)bump.height) % bump.height);
	hc = (c.r + c.g + c.b) / 3.0f;
	c = mlx_pixel_to_rgb(bump,
			(int)((hit.u + 1.0f / (float)bump.width)
				* (float)bump.width) % bump.width,
			(int)(hit.v * (float)bump.height) % bump.height);
	du = ((c.r + c.g + c.b) / 3.0f - hc) * 0.5f;
	c = mlx_pixel_to_rgb(bump,
			(int)(hit.u * (float)bump.width) % bump.width,
			(int)((hit.v + 1.0f / (float)bump.height)
				* (float)bump.height) % bump.height);
	dv = ((c.r + c.g + c.b) / 3.0f - hc) * 0.5f;
	return (ft_fnormalize(ft_fvector3_sum(hit.normal, ft_fvector3_sum(
					ft_fvector3_scale(ft_fnormalize(ft_fcross_product(
								cy->normal, ft_fnormalize(hit.proj))), du),
					ft_fvector3_scale(cy->normal, dv)))));
}
