/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated: 2025/06/02 03:44:55 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb	get_base_color(t_cylinder *cy, t_fvector3 impact_point,
						t_pattern pattern);
static inline t_rgb	display_texture(t_mlx_image texture, t_cylinder *cy,
						t_fvector3 diff,
						float h);
/* -------------------------------------------------------------------------- */

void	render_cylinder(t_minirt *mrt, t_ray *ray, t_object *object, int depth)
{
	float		dist;
	t_cylinder	*cylinder;
	t_hit_data	hit;
	int			inside;
	t_ray		reflect_ray;

	dist = intersect_cylinder(ray, object, 1.0f);
	if (dist < 0.0f || dist > ray->dist)
		return ;
	cylinder = (t_cylinder *)object;
	inside = init_cylinder(ray, &hit, cylinder, dist);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			get_base_color(cylinder, hit.impact_point, cylinder->pattern));
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
	ray->dist = dist;
}

static inline t_rgb	get_base_color(t_cylinder *cy, t_fvector3 impact_point,
	t_pattern pattern)
{
	t_fvector3	diff;
	t_fvector3	proj;
	float		h;
	float		angle;

	if (pattern.id != 'c' && cy->pattern.path == NULL)
		return (pattern.main_color);
	diff = ft_fvector3_diff(impact_point, cy->position);
	h = ft_fdot_product(diff, cy->normal);
	if (pattern.id == 'c')
	{
		proj = ft_fvector3_diff(diff, ft_fvector3_scale(cy->normal, h));
		angle = atan2f(ft_fdot_product(proj, cy->up),
				ft_fdot_product(proj, cy->right));
		if (angle < 0.0f)
			angle += 2.0f * M_PI;
		if ((int)(floor(angle * 3.0f + EPSILON)
			+ floor((h + cy->half_height) * 0.3f + EPSILON)) % 2 == 0)
			return (pattern.secondary_color);
	}
	if (cy->pattern.path != NULL)
		return (display_texture(cy->pattern.texture, cy, diff, h));
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_mlx_image texture, t_cylinder *cy,
	t_fvector3 diff, float h)
{
	float		u;
	float		v;
	t_fvector3	proj;
	float		angle;

	if (fabsf(fabsf(h) - cy->half_height) < EPSILON)
	{
		u = ft_fdot_product(diff, cy->right) / (2.0f * cy->radius) + 0.5f;
		v = ft_fdot_product(diff, cy->up) / (2.0f * cy->radius) + 0.5f;
	}
	else
	{
		proj = ft_fvector3_diff(diff, ft_fvector3_scale(cy->normal, h));
		angle = atan2f(ft_fdot_product(proj, cy->up),
				ft_fdot_product(proj, cy->right));
		if (angle < 0.0f)
			angle += 2.0f * M_PI;
		u = (angle / (2.0f * M_PI)) * 2.5f;
		v = ((h + cy->half_height) / (2.0f * cy->half_height));
	}
	u -= floorf(u);
	v -= floorf(v);
	return (mlx_pixel_to_rgb(texture,
			(int)(u * texture.width) % texture.width,
		(int)(v * texture.height) % texture.height));
}
