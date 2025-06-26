/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 00:20:45 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/26 19:46:27 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb	get_base_color(t_cone *cone, t_pattern pattern,
						t_fvector3 impact_point);
static inline t_rgb	display_texture(t_mlx_image texture, t_cone *cone,
						t_fvector3 diff,
						float h);
/* -------------------------------------------------------------------------- */

void	apply_lights_cone(t_minirt *mrt, t_ray *ray, t_object *object,
		int depth)
{
	t_cone		*cone;
	int			inside;
	t_rgb		base;
	t_hit_data	hit;
	t_ray		reflect_ray;

	cone = (t_cone *)object;
	inside = init_cone(ray, &hit, cone);
	base = get_base_color(cone, cone->pattern, hit.impact_point);
	if (base.r != 0 || base.g != 0 || base.b != 0)
		ray->color = apply_lights_modifier(
				get_lights_modifier(mrt, &hit, inside), base);
	if (!inside && cone->pattern.mattifying != 0.0f && hit.level != 0.0f)
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
	t_fvector3 impact_point)
{
	t_fvector3	diff;
	float		angle;
	float		h;

	if (pattern.id != 'c' && !pattern.path)
		return (pattern.main_color);
	diff = ft_fvector3_diff(impact_point, cone->position);
	if (pattern.id == 'c')
	{
		angle = atan2f(diff.x * cone->up.x + diff.y * cone->up.y + diff.z
				* cone->up.z, diff.x * cone->right.x + diff.y
				* cone->right.y + diff.z * cone->right.z);
		if (angle < 0.0f)
			angle += 2.0f * M_PI;
		if ((int)((floorf(angle * 3.0f + EPSILON)) + (floorf((diff.x
						* cone->normal.x + diff.y * cone->normal.y + diff.z
						* cone->normal.z) * 0.3f + EPSILON))) & 1)
			return (pattern.secondary_color);
	}
	h = diff.x * cone->normal.x + diff.y * cone->normal.y
		+ diff.z * cone->normal.z;
	if (pattern.path)
		return (display_texture(pattern.texture, cone, diff, h));
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_mlx_image texture, t_cone *cone,
	t_fvector3 diff, float h)
{
	float		u;
	float		v;
	t_fvector3	proj;
	float		angle;

	if (fabsf(fabsf(h) - cone->height) < EPSILON)
	{
		u = (diff.x * cone->right.x + diff.y * cone->right.y
				+ diff.z * cone->right.z) / cone->base_diameter + 0.5f;
		v = (diff.x * cone->up.x + diff.y * cone->up.y
				+ diff.z * cone->up.z) / cone->base_diameter + 0.5f;
	}
	else
	{
		proj = ft_fvector3_diff(diff, ft_fvector3_scale(cone->normal, h));
		angle = atan2f((proj.x * cone->up.x + proj.y * cone->up.y
					+ proj.z * cone->up.z), (proj.x * cone->right.x
					+ proj.y * cone->right.y + proj.z * cone->right.z));
		u = (angle + ((angle < 0.0f) * (2.0f * M_PI))) / (2.0f * M_PI);
		v = h / cone->height;
	}
	return (mlx_pixel_to_rgb(texture,
			(int)((u - floorf(u)) * texture.width) % texture.width,
		(int)((v - floorf(v)) * texture.height) % texture.height));
}
