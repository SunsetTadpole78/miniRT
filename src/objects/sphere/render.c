/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/05/30 14:28:18 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb	get_base_color(t_fvector3 normal, t_pattern pattern,
						t_sphere *sphere);
static inline t_rgb	display_texture(t_texture texture, t_fvector3 normal);
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
	if (!inside)
	{
		reflect_ray = *ray;
		specular_reflection(&reflect_ray, &hit, sphere->pattern.smoothness);
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
	if (pattern.id != 'c' && sphere->texture.image == NULL)
		return (pattern.main_color);
	if (sphere->texture.image != NULL)
		return (display_texture(sphere->texture, normal));
	if ((int)((floorf((0.5f + atan2f(normal.z, normal.x)
					/ (2.0f * M_PI)) * 10.0f))
		+ (floorf((0.5f - asinf(normal.y) / M_PI) * 10.0f))) % 2 == 0)
		return (pattern.secondary_color);
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_texture texture, t_fvector3 normal)
{
	float	u;
	float	v;

	u = 0.5f + atan2f(normal.z, normal.x) / (2.0f * M_PI);
	v = 0.5f - asinf(normal.y) / M_PI;
	return (texture_pixel_to_rgb(&texture,
			(int)(u * texture.width) % texture.width,
		(int)(v * texture.height) % texture.height));
}
