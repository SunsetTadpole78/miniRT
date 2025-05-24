/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:30:37 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/24 20:44:18 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

// Libraries

# include <fcntl.h>
# include <stdio.h>
# include <math.h>

# include "libft.h"
# include "mlx.h"

// Macros for Type Objects

# define AMBIANT_ID "A"
# define CAMERA_ID "C"
# define CYLINDER_ID "cy"
# define LIGHT_ID "L"
# define PLANE_ID "pl"
# define SPHERE_ID "sp"

// Macros for MLX

# define WIN_HEIGHT 700
# define WIN_WIDTH 1100

# define WINDOW_NAME "miniRT"

# define GAMMA 2.2f
# define LINEAR_ATTENUATION_COEF 0.0004f
# define QUADRATIC_ATTENUATION_COEF 0.004f

// Structures

typedef struct s_minirt	t_minirt;
typedef struct s_ray	t_ray;
typedef struct s_object	t_object;

typedef struct s_object
{
	char		*id;
	t_object	*next;
	void		(*render)(t_minirt *, t_ray *, t_object *);
}	t_object;

typedef struct s_color_object
{
	char		*id;
	t_object	*next;
	void		(*render)(t_minirt *, t_ray *, t_object *);
	t_rgb		color;
}	t_color_object;

typedef struct s_ambiant
{
	char		*id;
	t_object	*next;
	void		(*render)(t_minirt *, t_ray *, t_object *);
	t_rgb		color;
	float		level;
}	t_ambiant;

typedef struct s_ray
{
	t_fvector3		origin;
	t_fvector3		direction;
	float			dist;
	t_rgb			color;
}	t_ray;

typedef struct s_camera
{
	char		*id;
	t_object	*next;
	void		(*render)(t_minirt *, t_ray *, t_object *);
	t_fvector3	position;
	t_fvector3	normal;
	t_fvector3	right;
	t_fvector3	up;
	int			fov;
	float		iplane_scale;
}	t_camera;

typedef struct s_light
{
	char		*id;
	t_object	*next;
	void		(*render)(t_minirt *, t_ray *, t_object *);
	t_rgb		color;
	t_fvector3	position;
	float		level;
	t_rgb		render_color;
}	t_light;

typedef struct s_sphere
{
	char		*id;
	t_object	*next;
	void		(*render)(t_minirt *, t_ray *, t_object *);
	t_rgb		color;
	t_fvector3	position;
	float		diameter;
	float		radius;
}	t_sphere;

typedef struct s_plane
{
	char		*id;
	t_object	*next;
	void		(*render)(t_minirt *, t_ray *, t_object *);
	t_rgb		color;
	t_fvector3	position;
	t_fvector3	normal;
}	t_plane;

typedef struct s_cylinder
{
	char		*id;
	t_object	*next;
	void		(*render)(t_minirt *, t_ray *, t_object *);
	t_rgb		color;
	t_fvector3	position;
	t_fvector3	normal;
	t_fvector2	size;
	float		radius;
	float		half_height;
}	t_cylinder;

typedef struct s_mlx
{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img_ptr;
	char	*data;
	int		bpp;
	int		ll;
	int		cl;
	int		endian;
	int		update;
}	t_mlx;

typedef struct s_type
{
	char			*id;
	void			*(*parser)(char **);
	void			(*render)(t_minirt *, t_ray *, t_object *);
	struct s_type	*next;
}	t_type;

typedef struct s_minirt
{
	t_type		*types;
	t_object	*objects;
	t_light		*lights;
	t_ambiant	*ambiant;
	t_camera	*camera;
	t_mlx		*mlx;
}	t_minirt;

typedef struct s_hit_data
{
	t_object	*object;
	t_fvector3	impact_point;
	t_fvector3	normal;
	t_fvector3	position;
}	t_hit_data;

t_minirt	*minirt(void);
void		destruct_minirt(t_minirt *mrt, int destroy_mlx);

// mlx
void		init_mlx(t_mlx *mlx);
void		destruct_mlx(t_mlx *mlx);
int			key_hook(int keycode, t_minirt *mrt);
int			loop_hook(t_minirt *mrt);
int			close_window(t_minirt *mrt);
void		handle_events(t_minirt *mrt);

void		render_scene(t_minirt *mrt);

t_frgb		get_lights_modifier(t_minirt *mrt, t_hit_data hit, int inside,
				int (*check_method)(t_hit_data, t_fvector3));
t_rgb		apply_lights_modifier(t_frgb modifier, t_rgb base);

//objects
t_ambiant	*ambiant(float level, t_rgb color);
void		*parse_ambiant(char **values);

t_camera	*camera(t_fvector3 position, t_fvector3 normal, int fov);
void		*parse_camera(char **values);
void		update_yaw(t_camera *cam, float theta);
void		update_pitch(t_camera *cam, float theta);
void		update_fov(t_minirt *mrt, int incrementation);

t_cylinder	*cylinder(t_fvector3 position, t_fvector3 normal,
				t_fvector2 size, t_rgb color);
void		*parse_cylinder(char **values);
void		render_cylinder(t_minirt *mrt, t_ray *ray, t_object *object);
float		intersect_cap(t_fvector3 local_origin, t_fvector3 local_dir,
				float radius, float half_height);
void		normalize_side(t_fvector3 *local_origin, t_fvector3 *local_dir,
				t_ray ray, t_cylinder *cylinder);
float		apply_side_equation(t_fvector3 local_origin, t_fvector3 local_dir,
				t_cylinder *cylinder);
int			is_inside_cylinder(t_hit_data hit, t_fvector3 point);

t_light		*light(t_fvector3 position, float level, t_rgb color);
void		*parse_light(char **values);

t_plane		*plane(t_fvector3 position, t_fvector3 normal, t_rgb color);
void		*parse_plane(char **values);
void		render_plane(t_minirt *mrt, t_ray *ray, t_object *object);

t_sphere	*sphere(t_fvector3 position, float diameter, t_rgb color);
void		*parse_sphere(char **values);
void		render_sphere(t_minirt *mrt, t_ray *ray, t_object *object);
int			is_inside_sphere(t_hit_data hit, t_fvector3 point);

int			register_object(void *object);
int			register_light(t_light *light);
int			set_ambiant(t_ambiant *ambiant);
int			set_camera(t_camera *camera);

int			register_type(char *id, void *(*parser)(char **),
				void (*render)(t_minirt *, t_ray *, t_object *));
int			exist_type(char *id);
void		*get_parser_by_id(char *id);
void		*get_render_by_id(char *id);

//parsing
int			parse_map(char *path);
int			parse_fvector3(char *value, t_fvector3 *v3,
				char *invalid_format_error);
int			parse_normal(char *value, t_fvector3 *normal,
				char *invalid_format_error);
int			parse_color(char *value, t_rgb *rgb, char *invalid_format_error);
void		*error_and_null(char *error);

#endif
