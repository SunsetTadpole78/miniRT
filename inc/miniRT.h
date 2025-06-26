/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:30:37 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/26 14:02:56 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

// Libraries

# include <fcntl.h>
# include <stdio.h>
# include <math.h>
# include <pthread.h>
# include <semaphore.h>

# include "libft.h"
# include "mlx.h"

// Macros for Type Objects

# define AMBIANT_ID "A"
# define CAMERA_ID "C"
# define CONE_ID "co"
# define CYLINDER_ID "cy"
# define LIGHT_ID "l"
# define PLANE_ID "pl"
# define SPHERE_ID "sp"

// Macros for MLX

# define WIN_HEIGHT 700
# define WIN_WIDTH 1100

# define WINDOW_NAME "miniRT"

# define GAMMA 2.2f
# define EPSILON 0.001f
# define LINEAR_ATTENUATION_COEF 0.0004f
# define QUADRATIC_ATTENUATION_COEF 0.004f
# define RAD_ANGLE 0.785398f

# define MAX_DEPTH 100

// Structures

typedef struct s_minirt		t_minirt;
typedef struct s_ray		t_ray;
typedef struct s_object		t_object;
typedef struct s_camera		t_camera;
typedef struct s_type		t_type;
typedef struct s_methods	t_methods;

typedef struct s_mlx_image
{
	void	*ptr;
	char	*data;
	int		bpp;
	int		ll;
	int		cl;
	int		endian;
	int		height;
	int		width;
	float	ratio;
}	t_mlx_image;

typedef struct s_pattern
{
	char		id;
	t_rgb		main_color;
	t_rgb		secondary_color;
	float		smoothness;
	float		mattifying;
	float		smoothness_factor;
	char		*path;
	char		*bump_path;
	t_mlx_image	texture;
	t_mlx_image	bump;
}	t_pattern;

typedef struct s_object
{
	char		*id;
	t_object	*next;
	t_methods	*methods;
	int			selected;
	t_pattern	pattern;
	float		default_level;
}	t_object;

typedef struct s_normal_object
{
	char		*id;
	t_object	*next;
	t_methods	*methods;
	int			selected;
	t_pattern	pattern;
	float		default_level;
	t_fvector3	position;
	t_fvector3	normal;
}	t_normal_object;

typedef struct s_ambiant
{
	char		*id;
	t_object	*next;
	t_methods	*methods;
	int			selected;
	t_pattern	pattern;
	float		default_level;
	t_frgb		gamma_color;
	float		level;
}	t_ambiant;

typedef struct s_ray
{
	t_fvector3	origin;
	t_fvector3	direction;
	float		dist;
	t_rgb		color;
	int			extra;
	t_object	*object;
}	t_ray;

typedef struct s_camera
{
	char		*id;
	t_object	*next;
	t_methods	*methods;
	int			selected;
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
	t_methods	*methods;
	int			selected;
	t_pattern	pattern;
	float		default_level;
	t_fvector3	position;
	t_frgb		gamma_color;
	float		level;
	float		scale;
	float		radius;
	int			visible;
}	t_light;

typedef struct s_sphere
{
	char		*id;
	t_object	*next;
	t_methods	*methods;
	int			selected;
	t_pattern	pattern;
	float		default_level;
	t_fvector3	position;
	float		diameter;
	float		radius;
}	t_sphere;

typedef struct s_plane
{
	char		*id;
	t_object	*next;
	t_methods	*methods;
	int			selected;
	t_pattern	pattern;
	float		default_level;
	t_fvector3	position;
	t_fvector3	normal;
	t_fvector3	right;
	t_fvector3	up;
}	t_plane;

typedef struct s_cone
{
	char		*id;
	t_object	*next;
	t_methods	*methods;
	int			selected;
	t_pattern	pattern;
	float		default_level;
	t_fvector3	position;
	t_fvector3	normal;
	t_fvector3	right;
	t_fvector3	up;
	float		base_diameter;
	float		radius;
	float		height;
	int			infinite;
	float		k;
	float		k2;
}	t_cone;

typedef struct s_cylinder
{
	char		*id;
	t_object	*next;
	t_methods	*methods;
	int			selected;
	t_pattern	pattern;
	float		default_level;
	t_fvector3	position;
	t_fvector3	normal;
	t_fvector3	right;
	t_fvector3	up;
	float		diameter;
	float		radius;
	float		height;
	float		half_height;
	int			infinite;
}	t_cylinder;

typedef struct s_mlx
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_mlx_image	image;
	int			update;
	int			count;
}	t_mlx;

typedef struct s_type
{
	char		*id;
	t_methods	*methods;
	t_type		*next;
}	t_type;

typedef struct s_methods
{
	void		*(*parser)(char **);
	float		(*intersect)(t_ray *, t_object *, float);
	void		(*apply_lights)(t_minirt *, t_ray *, t_object *, int);
	void		(*on_press_key)(t_object *, int, t_camera *);
	int			(*is_inside)(t_object *, t_fvector3);
	t_object	*(*duplicate)(t_object *);
}	t_methods;

typedef struct s_thread_data
{
	t_minirt		*mrt;
	int				id;
	int				max;
	int				cores;
	t_camera		*camera;
	pthread_t		thread;
	float			ratio;
	int				count;
	pthread_mutex_t	update_mutex;
	pthread_cond_t	update_cond;
}	t_thread_data;

typedef struct s_minirt
{
	t_type			*types;
	t_object		*objects;
	t_light			*lights;
	t_ambiant		*ambiant;
	t_camera		*camera;
	t_mlx			*mlx;
	int				cores;
	int				pixels_per_thread;
	t_object		*selected;
	int				ctrl_pressed;
	t_object		*clipboard;
	t_thread_data	*threads_datas;
	int				threads_init;
	sem_t			*workers_sem;
	int				exit;
	pthread_mutex_t	exit_mutex;
}	t_minirt;

typedef struct s_hit_data
{
	t_object	*object;
	t_fvector3	impact_point;
	t_fvector3	normal;
	t_fvector3	position;
	float		level;
}	t_hit_data;

t_minirt	*minirt(void);
int			check_env(t_minirt *mrt);
void		init_render(t_minirt *mrt);
void		destruct_minirt(t_minirt *mrt, int destroy_mlx);

// mlx
t_mlx		*init_mlx(t_mlx *mlx);
void		destruct_mlx(t_mlx *mlx);
int			on_press_key(int keycode, t_minirt *mrt);
int			on_release_key(int keycode, t_minirt *mrt);
int			on_click(int id, int x, int y, t_minirt *mrt);
int			loop_hook(t_minirt *mrt);
int			close_window(t_minirt *mrt);
void		handle_events(t_minirt *mrt);
int			on_expose(t_mlx *mlx);

t_rgb		mlx_pixel_to_rgb(t_mlx_image image, int x, int y);

// render
void		render_scene(t_minirt *mrt);

t_rgb		ray_tracer(t_minirt *mrt, t_ray *ray, int depth);
t_fvector3	primary_ray(t_camera *cam, t_vector2 pos, float ratio);
t_frgb		get_lights_modifier(t_minirt *mrt, t_hit_data *hit, int inside);
void		blend_colors(t_minirt *mrt, t_ray *ray, t_vector2 pos, int count);
t_rgb		apply_lights_modifier(t_frgb modifier, t_rgb base);
void		apply_selection_effect(t_rgb *color);
void		specular_reflection(t_ray *ray, t_hit_data *hit,
				float smoothness_factor);

int			can_exit(t_minirt *mrt);
void		stop_threads(t_minirt *mrt);

t_fvector3	rotate_object(t_fvector3 v, t_fvector3 axis, float theta);

//objects
t_ambiant	*ambiant(float level, t_pattern pattern);
void		*parse_ambiant(char **values);

t_camera	*camera(t_fvector3 position, t_fvector3 normal, int fov);
void		*parse_camera(char **values);
void		on_press_key_camera(t_minirt *mrt, t_camera *camera, int keycode);

t_cone		*cone(t_fvector3 position, t_fvector3 normal,
				t_fvector2 size, t_pattern pattern);
void		*parse_cone(char **values);
void		apply_lights_cone(t_minirt *mrt, t_ray *ray, t_object *object,
				int depth);
float		intersect_cone(t_ray *ray, t_object *object, float amplifier);
int			init_cone(t_ray *ray, t_hit_data *hit, t_cone *cone);
int			is_inside_cone(t_object *object, t_fvector3 point);
void		on_press_key_cone(t_object *object, int keycode, t_camera *camera);
t_object	*duplicate_cone(t_object *object);

t_cylinder	*cylinder(t_fvector3 position, t_fvector3 normal,
				t_fvector2 size, t_pattern pattern);
void		*parse_cylinder(char **values);
void		apply_lights_cylinder(t_minirt *mrt, t_ray *ray, t_object *object,
				int depth);
float		intersect_cylinder(t_ray *ray, t_object *object, float amplifier);
int			init_cylinder(t_ray *ray, t_hit_data *hit,
				t_cylinder *cylinder);
float		intersect_cap(t_fvector3 o, t_fvector3 d, float radius,
				float half_height);
float		apply_side_equation(t_fvector3 o, t_fvector3 d,
				t_cylinder *cylinder, float amplifier);
int			is_inside_cylinder(t_object *object, t_fvector3 point);
void		on_press_key_cylinder(t_object *object, int keycode,
				t_camera *camera);
t_object	*duplicate_cylinder(t_object *object);

t_light		*light(t_fvector3 position, float level, t_pattern pattern,
				float scale);
void		*parse_light(char **values);
void		show_light(t_ray *ray, t_light *light);
float		intersect_light(t_ray *ray, t_object *object, float amplifier);
void		on_press_key_light(t_object *object, int keycode, t_camera *camera);
t_object	*duplicate_light(t_object *object);

t_plane		*plane(t_fvector3 position, t_fvector3 normal, t_pattern pattern);
void		*parse_plane(char **values);
void		apply_lights_plane(t_minirt *mrt, t_ray *ray, t_object *object,
				int depth);
float		intersect_plane(t_ray *ray, t_object *object, float amplifier);
void		on_press_key_plane(t_object *object, int keycode, t_camera *camera);
t_object	*duplicate_plane(t_object *object);

t_sphere	*sphere(t_fvector3 position, float diameter, t_pattern pattern);
void		*parse_sphere(char **values);
void		apply_lights_sphere(t_minirt *mrt, t_ray *ray, t_object *object,
				int depth);
float		intersect_sphere(t_ray *ray, t_object *object, float amplifier);
int			is_inside_sphere(t_object *object, t_fvector3 point);
void		on_press_key_sphere(t_object *object, int keycode,
				t_camera *camera);
t_object	*duplicate_sphere(t_object *object);

int			register_object(t_object *object);
int			register_light(t_light *light);
int			set_ambiant(t_ambiant *ambiant);
int			set_camera(t_camera *camera);

int			register_type(char *id, void *(*parser)(char **),
				float (*intersect)(t_ray *, t_object *, float),
				t_methods *methods);
int			exist_type(char *id);
t_methods	*init_methods(void (*apply_lights)(t_minirt *, t_ray *, t_object *,
					int),
				int (*is_inside)(t_object *, t_fvector3),
				void (*on_press_key)(t_object *, int, t_camera *),
				t_object *(*duplicate)(t_object *));
t_methods	*empty_methods(void);
t_methods	*get_methods_by_id(char *id);

void		normalize_complex_object(t_fvector3 *o, t_fvector3 *d, t_ray ray,
				t_normal_object *object);

void		delete_object(t_minirt *mrt, t_object *object);

//parsing
int			parse_map(char *path);
int			parse_fvector3(char *value, t_fvector3 *v3);
int			parse_normal(char *value, t_fvector3 *normal);
int			parse_color(char *value, t_rgb *rgb);
int			parse_texture(char *value, t_pattern *pattern);
int			parse_pattern(char **values, t_pattern *pattern);
void		*error_and_null(char *error);
void		init_pattern(t_pattern *pattern);
void		init_texture(t_mlx_image *texture, char *path, void *ptr);

#endif
