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
static inline void	init_hit(t_ray *ray, t_hit_data *hit, t_sphere *sphere,
						float dist);
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
	init_hit(ray, &hit, sphere, dist);
	inside = ft_fvector3_length(ft_fvector3_diff(ray->origin,
				sphere->position)) < sphere->radius;
	if (inside)
		hit.normal = ft_fvector3_scale(hit.normal, -1);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			get_base_color(hit.normal, sphere->pattern, sphere));
	reflect_ray = *ray;
	specular_reflection(&reflect_ray, &hit, sphere->pattern.smoothness);
	ray->color = ft_rgb_lerp(ray->color, ray_tracer(mrt, &reflect_ray,
				depth + 1), sphere->pattern.mattifying);
	if (sphere->selected)
		apply_selection_effect(&ray->color);
	ray->dist = dist;
}

float	intersect_sphere(t_ray *ray, t_object *object, float amplifier)
{
	t_sphere	*sphere;
	t_fvector3	oc;
	float		b;
	float		delta;
	float		x;

	sphere = (t_sphere *)object;
	oc = ft_fvector3_diff(ray->origin, sphere->position);
	b = 2.0f * ft_fdot_product(oc, ray->direction);
	delta = b * b - 4.0f * (ft_fdot_product(oc, oc)
			- (sphere->radius * sphere->radius * amplifier));
	if (delta < 0.0f)
		return (-1.0f);
	x = (-b - sqrtf(delta)) / 2.0f;
	if (x > EPSILON)
		return (x);
	x = (-b + sqrtf(delta)) / 2.0f;
	if (x > EPSILON)
		return (x);
	return (-1.0f);
}

static inline void	init_hit(t_ray *ray, t_hit_data *hit, t_sphere *sphere,
	float dist)
{
	hit->object = (t_object *)sphere;
	hit->impact_point = ft_fvector3_sum(ray->origin,
			ft_fvector3_scale(ray->direction, dist));
	hit->normal = ft_fnormalize(ft_fvector3_diff(hit->impact_point,
				sphere->position));
	hit->position = sphere->position;
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
