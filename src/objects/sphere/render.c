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
static inline int		init_sphere(t_ray *ray, t_hit_data *hit,
							t_sphere *sphere);
static inline t_rgb	get_base_color(t_pattern pattern, t_fvector3 normal,
							int inside);
static inline t_rgb	display_texture(t_mlx_image texture, float u, float v);
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
			get_base_color(sphere->pattern, hit.normal, inside));
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

static inline int	init_sphere(t_ray *ray, t_hit_data *hit, t_sphere *sphere)
{
	int	inside;

	hit->object = (t_object *)sphere;
	hit->impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, ray->dist));
	hit->normal = ft_fnormalize(ft_fvector3_diff(hit->impact_point,
				sphere->position));
	hit->position = sphere->position;
	inside = ft_fvector3_length(ft_fvector3_diff(ray->origin,
				sphere->position)) < sphere->radius;
	if (inside)
		hit->normal = ft_fvector3_scale(hit->normal, -1);
	return (inside);
}

static inline t_rgb	get_base_color(t_pattern pattern, t_fvector3 normal,
	int inside)
{
	float	u;
	float	v;

	if (pattern.id != 'c' && !pattern.path)
		return (pattern.main_color);
	if (inside)
		normal = ft_fvector3_scale(normal, -1);
	u = 0.5f + atan2f(normal.z, normal.x) / (2.0f * M_PI);
	v = 0.5f - asinf(normal.y) / M_PI;
	if (pattern.id == 'c'
		&& (int)((floorf(u * 10.0f)) + (floorf(v * 10.0f))) & 1)
		return (pattern.secondary_color);
	if (pattern.path)
		return (display_texture(pattern.texture, u, v));
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_mlx_image texture, float u, float v)
{
	return (mlx_pixel_to_rgb(texture,
			(int)(u * texture.width) % texture.width,
		(int)(v * texture.height) % texture.height));
}

/*
static inline t_fvector3	bump_mapping(t_mlx_image bump, t_fvector3 normal,
	float u, float v)
{
	t_rgb	c;
	float	intensity;
	float	offset;

	c = display_texture(bump, u, v);
	intensity = (c.r + c.g + c.b) / (3.0f * 255.0f);
	offset = 0.8f * (intensity - 0.5f);
	return (ft_fnormalize(ft_fvector3_sum(normal,
			(t_fvector3){offset, offset, offset})));
}
*/
