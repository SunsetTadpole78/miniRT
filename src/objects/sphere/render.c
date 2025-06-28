/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/26 19:46:52 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline int			init_sphere(t_ray *ray, t_hit_data *hit,
								t_sphere *sphere);
static inline t_rgb		get_base_color(t_pattern pattern, t_hit_data hit,
								int inside);
static inline t_rgb		display_texture(t_mlx_image texture, float u, float v);
static inline t_fvector3	bump_mapping(t_mlx_image bump, t_hit_data hit);
/* -------------------------------------------------------------------------- */

void	apply_lights_sphere(t_minirt *mrt, t_ray *ray, t_object *object,
		int depth)
{
	t_sphere	*sphere;
	t_hit_data	hit;
	int			inside;
	t_rgb		base;
	t_ray		reflect_ray;

	sphere = (t_sphere *)object;
	inside = init_sphere(ray, &hit, sphere);
	if (sphere->pattern.bump_path)
		hit.normal = bump_mapping(sphere->pattern.bump, hit);
	base = get_base_color(sphere->pattern, hit, inside);
	if (base.r != 0 || base.g != 0 || base.b != 0)
		ray->color = apply_lights_modifier(
				get_lights_modifier(mrt, &hit, inside), base);
	if (!inside && sphere->pattern.mattifying != 0.0f && hit.level != 0.0f)
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
	hit->diff = (t_fvector3){0.0f, 0.0f, 0.0f};
	hit->proj = (t_fvector3){0.0f, 0.0f, 0.0f};
	hit->u = 0.5f + atan2f(hit->normal.z, hit->normal.x) / (2.0f * M_PI);
	hit->v = 0.5f - asinf(hit->normal.y) / M_PI;
	hit->h = 0.0f;
	inside = ft_fvector3_length(ft_fvector3_diff(ray->origin,
				sphere->position)) < sphere->radius;
	if (inside)
		hit->normal = ft_fvector3_scale(hit->normal, -1);
	hit->level = sphere->default_level;
	return (inside);
}

static inline t_rgb	get_base_color(t_pattern pattern, t_hit_data hit,
	int inside)
{
	if (pattern.id != 'c' && !pattern.path)
		return (pattern.main_color);
	if (inside)
		hit.normal = ft_fvector3_scale(hit.normal, -1);
	if (pattern.id == 'c'
		&& (int)((floorf(hit.u * 10.0f)) + (floorf(hit.v * 10.0f))) & 1)
		return (pattern.secondary_color);
	if (pattern.path)
		return (display_texture(pattern.texture, hit.u, hit.v));
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_mlx_image texture, float u, float v)
{
	return (mlx_pixel_to_rgb(texture,
			(int)(u * texture.width) % texture.width,
		(int)(v * texture.height) % texture.height));
}

static inline t_fvector3	bump_mapping(t_mlx_image bump, t_hit_data hit)
{
	t_rgb		c;
	float		du;
	float		dv;
	float		hc;
	t_fvector3	t;

	c = display_texture(bump, hit.u, hit.v);
	hc = (c.r + c.g + c.b) / 3.0f;
	c = display_texture(bump, hit.u + 0.9f / (float)bump.width, hit.v);
	du = ((c.r + c.g + c.b) / 3.0f - hc) * 0.75;
	c = display_texture(bump, hit.u, hit.v + 0.9f / (float)bump.height);
	dv = ((c.r + c.g + c.b) / 3.0f - hc) * 0.75;
	t = ft_fnormalize(
			ft_fcross_product((t_fvector3){0.0f, 1.0f, 0.0f}, hit.normal));
	return (ft_fnormalize(ft_fvector3_sum(hit.normal, ft_fvector3_sum(
					ft_fvector3_scale(t, du), ft_fvector3_scale(ft_fnormalize(
							ft_fcross_product(hit.normal, t)), dv)))));
}
