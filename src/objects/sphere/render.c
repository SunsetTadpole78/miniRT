/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/03 02:47:42 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb	get_base_color(t_pattern pattern, t_fvector3 normal,
						t_hit_data hit, int inside);
static inline t_rgb	display_texture(t_mlx_image texture, t_hit_data hit);
/* -------------------------------------------------------------------------- */

void	apply_lights_sphere(t_minirt *mrt, t_ray *ray, t_object *object,
		int depth)
{
	t_sphere	*sphere;
	t_hit_data	hit;
	int			inside;
	t_ray		reflect_ray;

	sphere = (t_sphere *)object;
	inside = init_sphere(ray, &hit, sphere);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			get_base_color(sphere->pattern, hit.normal, hit, inside));
	if (!inside && sphere->pattern.mattifying != 0.0f)
	{
		reflect_ray = *ray;
		specular_reflection(&reflect_ray, &hit,
			sphere->pattern.smoothness_factor);
		ray->color = ft_rgb_lerp(ray->color, ray_tracer(mrt, &reflect_ray,
					depth + 1), sphere->pattern.mattifying);
	}
	if (sphere->selected)
		apply_selection_effect(&ray->color);
}

static inline t_rgb	get_base_color(t_pattern pattern, t_fvector3 normal,
	t_hit_data hit, int inside)
{
	if (pattern.id != 'c' && !pattern.path)
		return (pattern.main_color);
	if (inside)
		normal = ft_fvector3_scale(normal, -1);
	hit.u = 0.5f + atan2f(hit.normal.z, hit.normal.x) / (2.0f * M_PI);
	hit.v = 0.5f - asinf(hit.normal.y) / M_PI;
	if (pattern.id == 'c'
		&& (int)((floorf(hit.u * 10.0f)) + (floorf(hit.v * 10.0f))) & 1)
		return (pattern.secondary_color);
	if (pattern.path)
		return (display_texture(pattern.texture, hit));
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_mlx_image texture, t_hit_data hit)
{
	return (mlx_pixel_to_rgb(texture,
			(int)(hit.u * texture.width) % texture.width,
		(int)(hit.v * texture.height) % texture.height));
}
