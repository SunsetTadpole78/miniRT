/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 00:20:45 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/25 13:01:31 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb	get_base_color(t_cone *cone, t_pattern pattern,
						t_hit_data hit);
static inline t_rgb	display_texture(t_mlx_image texture, t_cone *cone,
						t_hit_data hit);
/* -------------------------------------------------------------------------- */

void	apply_lights_cone(t_minirt *mrt, t_ray *ray, t_object *object,
		int depth)
{
	t_cone		*cone;
	int			inside;
	t_hit_data	hit;
	t_ray		reflect_ray;

	cone = (t_cone *)object;
	inside = init_cone(ray, &hit, cone);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			get_base_color(cone, cone->pattern, hit));
	if (!inside && cone->pattern.mattifying != 0.0f)
	{
		reflect_ray = *ray;
		specular_reflection(&reflect_ray, &hit,
			cone->pattern.smoothness_factor);
		ray->color = ft_rgb_lerp(ray->color, ray_tracer(mrt, &reflect_ray,
					depth + 1), cone->pattern.mattifying);
	}
	if (cone->selected)
		apply_selection_effect(&ray->color);
}

static inline t_rgb	get_base_color(t_cone *cone, t_pattern pattern,
	t_hit_data hit)
{
	float		angle;

	if (pattern.id != 'c' && !pattern.path)
		return (pattern.main_color);
	if (pattern.id == 'c')
	{
		hit.diff = ft_fvector3_diff(hit.impact_point, hit.position);
		hit.h = hit.diff.x * cone->normal.x + hit.diff.y * cone->normal.y
			+ hit.diff.z * cone->normal.z;
		angle = atan2f(hit.diff.x * cone->up.x + hit.diff.y * cone->up.y
				+ hit.diff.z * cone->up.z, hit.diff.x * cone->right.x
				+ hit.diff.y * cone->right.y + hit.diff.z * cone->right.z);
		if ((int)(floorf((angle + (angle < 0.0f) * 2.0f * M_PI)
				* 3.0f + EPSILON) + floorf(hit.h * 0.3f + EPSILON)) & 1)
			return (pattern.secondary_color);
	}
	if (pattern.path)
		return (display_texture(pattern.texture, cone, hit));
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_mlx_image texture, t_cone *cone,
	t_hit_data hit)
{
	float		angle;

	hit.diff = ft_fvector3_diff(hit.impact_point, hit.position);
	hit.h = hit.diff.x * cone->normal.x + hit.diff.y * cone->normal.y
		+ hit.diff.z * cone->normal.z;
	if (fabsf(fabsf(hit.h) - cone->height) < EPSILON)
	{
		hit.u = (hit.diff.x * cone->right.x + hit.diff.y * cone->right.y
				+ hit.diff.z * cone->right.z) / cone->base_diameter + 0.5f;
		hit.v = (hit.diff.x * cone->up.x + hit.diff.y * cone->up.y
				+ hit.diff.z * cone->up.z) / cone->base_diameter + 0.5f;
	}
	else
	{
		hit.proj = ft_fvector3_diff(hit.diff,
				ft_fvector3_scale(cone->normal, hit.h));
		angle = atan2f((hit.proj.x * cone->up.x + hit.proj.y * cone->up.y
					+ hit.proj.z * cone->up.z), (hit.proj.x * cone->right.x
					+ hit.proj.y * cone->right.y + hit.proj.z * cone->right.z));
		hit.u = (angle + ((angle < 0.0f) * (2.0f * M_PI))) / (2.0f * M_PI);
		hit.v = hit.h / cone->height;
	}
	return (mlx_pixel_to_rgb(texture,
			(int)((hit.u - floorf(hit.u)) * texture.width) % texture.width,
		(int)((hit.v - floorf(hit.v)) * texture.height) % texture.height));
}
