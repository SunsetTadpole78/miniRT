/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 00:20:45 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/31 21:10:16 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ------------------------------- PROTOTYPE -------------------------------- */
static inline t_rgb	get_base_color(t_cone *co, t_fvector3 impact_point,
						t_pattern pattern);
static inline t_rgb	display_texture(t_texture texture, t_cone *cone,
						t_fvector3 diff, float h);
/* -------------------------------------------------------------------------- */

void	render_cone(t_minirt *mrt, t_ray *ray, t_object *object, int depth)
{
	float		dist;
	t_cone		*cone;
	t_hit_data	hit;
	int			inside;
	t_ray		reflect_ray;

	dist = intersect_cone(ray, object, 1.0f);
	if (dist < 0.0f || dist > ray->dist)
		return ;
	cone = (t_cone *)object;
	inside = init_cone(ray, &hit, cone, dist);
	ray->color = apply_lights_modifier(get_lights_modifier(mrt, hit, inside),
			get_base_color(cone, hit.impact_point, cone->pattern));
	if (!inside)
	{
		reflect_ray = *ray;
		specular_reflection(&reflect_ray, &hit, cone->pattern.smoothness);
		ray->color = ft_rgb_lerp(ray->color, ray_tracer(mrt, &reflect_ray,
					depth + 1), cone->pattern.mattifying);
	}
	if (cone->selected)
		apply_selection_effect(&ray->color);
	ray->dist = dist;
}

static inline t_rgb	get_base_color(t_cone *cone, t_fvector3 impact_point,
		t_pattern pattern)
{
	t_fvector3	diff;
	t_fvector3	local;
	float		angle;
	float		h;

	if (pattern.id != 'c' && cone->texture.image == NULL)
		return (pattern.main_color);
	diff = ft_fvector3_diff(impact_point, cone->position);
	if (cone->texture.image != NULL)
	{
		h = ft_fdot_product(diff, cone->normal);
		return (display_texture(cone->texture, cone, diff, h));
	}
	local = (t_fvector3){
		ft_fdot_product(diff, cone->right),
		ft_fdot_product(diff, ft_fnormalize(cone->normal)),
		ft_fdot_product(diff, cone->up)
	};
	angle = atan2f(local.z, local.x);
	if (angle < 0.0f)
		angle += 2.0f * M_PI;
	if (((int)(floor(angle * 3.0f + EPSILON))
		+ (int)(floor(local.y * 0.3f + EPSILON))) % 2 == 0)
		return (pattern.secondary_color);
	return (pattern.main_color);
}

static inline t_rgb	display_texture(t_texture texture, t_cone *cone,
	t_fvector3 diff, float h)
{
	float		u;
	float		v;
	t_fvector3	proj;
	float		angle;

	if (fabsf(h - cone->height) < EPSILON)
	{
		u = ft_fdot_product(diff, cone->right) / (2.0f * cone->radius) + 0.5f;
		v = ft_fdot_product(diff, cone->up) / (2.0f * cone->radius) + 0.5f;
	}
	else
	{
		proj = ft_fvector3_diff(diff, ft_fvector3_scale(cone->normal, h));
		angle = atan2f(ft_fdot_product(proj, cone->up),
				ft_fdot_product(proj, cone->right));
		if (angle < 0.0f)
			angle += 2.0f * M_PI;
		u = angle / (2.0f * M_PI);
		v = h / cone->height;
	}
	u -= floorf(u);
	v -= floorf(v);
	return (texture_pixel_to_rgb(&texture,
			(int)(u * texture.width) % texture.width,
		(int)(v * texture.height) % texture.height));
}
