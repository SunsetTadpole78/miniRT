/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/03 02:52:20 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline void	init_plane(t_ray *ray, t_hit_data *hit, t_plane *plane);
static inline t_rgb	get_base_color(t_pattern pattern, t_hit_data hit);
static inline t_rgb	display_texture(t_mlx_image texture, t_hit_data hit);
/* -------------------------------------------------------------------------- */

void	apply_lights_plane(t_minirt *mrt, t_ray *ray, t_object *object,
		int depth)
{
	t_plane		*plane;
	t_hit_data	hit;
	t_ray		reflect_ray;

	plane = (t_plane *)object;
	init_plane(ray, &hit, plane);
	if (ft_fdot_product(ray->direction, hit.normal) > 0)
		hit.normal = ft_fvector3_scale(hit.normal, -1);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, 0),
			get_base_color(plane->pattern, hit));
	if (plane->pattern.mattifying != 0.0f)
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
	hit->u = ft_fdot_product(hit->diff, plane->right) * 0.05f;
	hit->v = ft_fdot_product(hit->diff, plane->up) * 0.05f;
}

static inline t_rgb	get_base_color(t_pattern pattern, t_hit_data hit)
{
	if (pattern.id != 'c' && !pattern.path)
		return (pattern.main_color);
	if (pattern.id == 'c' && (int)((floorf(hit.u)) + (floorf(hit.v))) & 1)
		return (pattern.secondary_color);
	if (pattern.path)
		return (display_texture(pattern.texture, hit));
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_mlx_image texture, t_hit_data hit)
{
	float	v;

	v = hit.v * texture.ratio;
	return (mlx_pixel_to_rgb(texture,
			(int)((hit.u - floorf(hit.u)) * texture.width) % texture.width,
		(int)((v - floorf(v)) * texture.height) % texture.height));
}
