/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/26 19:46:46 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void		init_plane(t_ray *ray, t_hit_data *hit, t_plane *plane);
static inline t_rgb		get_base_color(t_pattern pattern, t_hit_data hit);
static inline t_rgb		display_texture(t_mlx_image texture, float u, float v);
static inline t_fvector3	bump_mapping(t_plane *plane, t_mlx_image bump,
							t_hit_data hit);
/* -------------------------------------------------------------------------- */

void	apply_lights_plane(t_minirt *mrt, t_ray *ray, t_object *object,
		int depth)
{
	t_plane		*plane;
	t_hit_data	hit;
	t_rgb		base;
	t_ray		reflect_ray;

	plane = (t_plane *)object;
	init_plane(ray, &hit, plane);
	if ((ray->direction.x * hit.normal.x + ray->direction.y * hit.normal.y
			+ ray->direction.z * hit.normal.z) > 0)
		hit.normal = ft_fvector3_scale(hit.normal, -1);
	base = get_base_color(plane->pattern, hit);
	if (base.r != 0 || base.g != 0 || base.b != 0)
		ray->color = apply_lights_modifier(
				get_lights_modifier(mrt, &hit, 0), base);
	if (plane->pattern.mattifying != 0.0f && hit.level != 0.0f)
	{
		reflect_ray = *ray;
		specular_reflection(&reflect_ray, &hit,
			plane->pattern.smoothness_factor);
		ray->color = ft_rgb_lerp(ray->color, ray_tracer(mrt, &reflect_ray,
					depth + 1), plane->pattern.mattifying);
	}
	if (plane->selected)
		apply_selection_effect(&ray->color);
}

static inline void	init_plane(t_ray *ray, t_hit_data *hit, t_plane *plane)
{
	hit->object = (t_object *)plane;
	hit->impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, ray->dist));
	hit->normal = plane->normal;
	hit->position = plane->position;
	hit->diff = ft_fvector3_diff(hit->impact_point, hit->position);
	hit->proj = (t_fvector3){0.0f, 0.0f, 0.0f};
	hit->u = (hit->diff.x * plane->right.x + hit->diff.y
			* plane->right.y + hit->diff.z * plane->right.z) * 0.03f;
	hit->v = (hit->diff.x * plane->up.x + hit->diff.y
			* plane->up.y + hit->diff.z * plane->up.z) * 0.03f;
	hit->h = 0.0f;
	hit->level = plane->default_level;
	if (plane->pattern.bump_path)
		hit->normal = bump_mapping(plane, plane->pattern.bump, *hit);
	if ((ray->direction.x * hit->normal.x + ray->direction.y * hit->normal.y
			+ ray->direction.z * hit->normal.z) > 0)
		hit->normal = ft_fvector3_scale(hit->normal, -1);
}

static inline t_rgb	get_base_color(t_pattern pattern, t_hit_data hit)
{
	if (pattern.id != 'c' && !pattern.path)
		return (pattern.main_color);
	if (pattern.id == 'c' && (int)((floorf(hit.u)) + (floorf(hit.v))) & 1)
		return (pattern.secondary_color);
	if (pattern.path)
		return (display_texture(pattern.texture, hit.u, hit.v));
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_mlx_image texture, float u, float v)
{
	v *= texture.ratio;
	return (mlx_pixel_to_rgb(texture,
			(int)((u - floorf(u)) * texture.width) % texture.width,
		(int)((v - floorf(v)) * texture.height) % texture.height));
}

static inline t_fvector3	bump_mapping(t_plane *plane, t_mlx_image bump,
	t_hit_data hit)
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
	t = ft_fnormalize(plane->right);
	return (ft_fnormalize(ft_fvector3_sum(hit.normal, ft_fvector3_sum(
					ft_fvector3_scale(t, du),
					ft_fvector3_scale(plane->up, dv)))));
}
