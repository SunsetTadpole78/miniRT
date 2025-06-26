/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/03 00:28:44 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb	get_base_color(t_cylinder *cy, t_pattern pattern,
						t_hit_data hit);
static inline t_rgb	display_texture(t_mlx_image texture, t_cylinder *cy,
						t_hit_data hit);
/* -------------------------------------------------------------------------- */

void	apply_lights_cylinder(t_minirt *mrt, t_ray *ray, t_object *object,
		int depth)
{
	t_cylinder	*cylinder;
	t_hit_data	hit;
	int			inside;
	t_ray		reflect_ray;

	cylinder = (t_cylinder *)object;
	inside = init_cylinder(ray, &hit, cylinder);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			get_base_color(cylinder, cylinder->pattern, hit));
	if (!inside && cylinder->pattern.mattifying != 0.0f)
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
		hit.diff = ft_fvector3_diff(hit.impact_point, hit.position);
		hit.h = ft_fdot_product(hit.diff, cy->normal);
		hit.proj = ft_fvector3_diff(hit.diff,
				ft_fvector3_scale(cy->normal, hit.h));
		angle = atan2f(ft_fdot_product(hit.proj, cy->up),
				ft_fdot_product(hit.proj, cy->right));
		if (angle < 0.0f)
			angle += 2.0f * M_PI;
		if ((int)(floorf(angle * 3.0f + EPSILON)
			+ floorf((hit.h + cy->half_height) * 0.3f + EPSILON)) & 1)
			return (pattern.secondary_color);
	}
	if (pattern.path)
		return (display_texture(pattern.texture, cy, hit));
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_mlx_image texture, t_cylinder *cy,
	t_hit_data hit)
{
	float		u;
	float		v;
	float		angle;

	hit.diff = ft_fvector3_diff(hit.impact_point, hit.position);
	hit.h = ft_fdot_product(hit.diff, cy->normal);
	if (fabsf(fabsf(hit.h) - cy->half_height) < EPSILON)
	{
		u = ft_fdot_product(hit.diff, cy->right) / cy->diameter + 0.5f;
		v = ft_fdot_product(hit.diff, cy->up) / cy->diameter + 0.5f;
	}
	else
	{
		hit.proj = ft_fvector3_diff(hit.diff,
				ft_fvector3_scale(cy->normal, hit.h));
		angle = atan2f(ft_fdot_product(hit.proj, cy->up),
				ft_fdot_product(hit.proj, cy->right));
		if (angle < 0.0f)
			angle += 2.0f * M_PI;
		u = angle / (2.0f * M_PI);
		v = (hit.h + cy->half_height) / cy->height;
	}
	return (mlx_pixel_to_rgb(texture,
			(int)((u - floorf(u)) * texture.width) % texture.width,
		(int)((v - floorf(v)) * texture.height) % texture.height));
}
