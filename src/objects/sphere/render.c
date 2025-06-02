/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/02 03:45:30 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb	get_base_color(t_fvector3 normal, t_pattern pattern,
						t_sphere *sphere);
static inline t_rgb	display_texture(t_mlx_image texture, float u, float v);
/* -------------------------------------------------------------------------- */

void	render_sphere(t_minirt *mrt, t_ray *ray, t_object *object, int depth)
{
	t_ray		reflect_ray;
	float		dist;
	t_sphere	*sphere;
	t_hit_data	hit;
	int			inside;

	dist = intersect_sphere(ray, object, 1.0f);
	if (dist <= 0 || dist > ray->dist)
		return ;
	sphere = (t_sphere *)object;
	inside = init_sphere(ray, &hit, sphere, dist);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			get_base_color(hit.normal, sphere->pattern, sphere));
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
	ray->dist = dist;
}

static inline t_rgb	get_base_color(t_fvector3 normal, t_pattern pattern,
	t_sphere *sphere)
{
	float	u;
	float	v;

	if (pattern.id != 'c' && sphere->pattern.path == NULL)
		return (pattern.main_color);
	u = 0.5f + atan2f(normal.z, normal.x) / (2.0f * M_PI);
	v = 0.5f - asinf(normal.y) / M_PI;
	if (pattern.id == 'c'
		&& (int)((floorf(u * 10.0f)) + (floorf(v * 10.0f))) % 2 == 0)
		return (pattern.secondary_color);
	if (sphere->pattern.path != NULL)
		return (display_texture(sphere->pattern.texture, u, v));
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_mlx_image texture, float u, float v)
{
	return (mlx_pixel_to_rgb(texture,
			(int)(u * texture.width) % texture.width,
		(int)(v * texture.height) % texture.height));
}
