/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:30:37 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/20 19:00:53 by lroussel         ###   ########.fr       */
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

// Structures

typedef struct s_minirt	t_minirt;
typedef struct s_ray	t_ray;
typedef struct s_object	t_object;

typedef struct s_object
{
	char		*id;
	t_object	*next;
	void		(*render)(t_minirt *, t_ray *,t_object *);
}	t_object;

typedef struct s_color_object
{
	char		*id;
	t_object	*next;
	void		(*render)(t_minirt *, t_ray *,t_object *);
	t_rgb		color;
}	t_color_object;

typedef struct s_ambiant
{
	char		*id;
	t_object	*next;
	void		(*render)(t_minirt *, t_ray *,t_object *);
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
	void		(*render)(t_minirt *, t_ray *,t_object *);
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
	void		(*render)(t_minirt *, t_ray *,t_object *);
	t_rgb		color;
	t_fvector3	position;
	float		level;
	float		linear_at_coef;
	float		quadratic_at_coef;
	t_rgb		render_color;
}	t_light;

typedef struct s_sphere
{
	char		*id;
	t_object	*next;
	void		(*render)(t_minirt *, t_ray *,t_object *);
	t_rgb		color;
	t_fvector3	position;
	float		diameter;
	float		radius;
}	t_sphere;

typedef struct s_plane
{
	char		*id;
	t_object	*next;
	void		(*render)(t_minirt *, t_ray *,t_object *);
	t_rgb		color;
	t_fvector3	position;
	t_fvector3	normal;
}	t_plane;

typedef struct s_cylinder
{
	char		*id;
	t_object	*next;
	void		(*render)(t_minirt *, t_ray *,t_object *);
	t_rgb		color;
	t_fvector3	position;
	t_fvector3	normal;
	t_fvector2	size;
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
	void			(*updater)(t_minirt *, t_object *);
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

t_rgb		calculate_brightness(t_minirt *mrt, t_fvector3 impact_point,
				t_fvector3 normal, t_rgb rgb);

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

t_light		*light(t_fvector3 position, float level, t_rgb color);
void		*parse_light(char **values);
void		render_light(t_minirt *mrt, t_ray *ray,
				t_object *object);

t_plane		*plane(t_fvector3 position, t_fvector3 normal, t_rgb color);
void		*parse_plane(char **values);
void		render_plane(t_minirt *mrt, t_ray *ray,
				t_object *object);

t_sphere	*sphere(t_fvector3 position, float diameter, t_rgb color);
void		*parse_sphere(char **values);
void		render_sphere(t_minirt *mrt, t_ray *ray,
				t_object *object);

int			register_object(void *object);
int			register_light(t_light *light);
int			set_ambiant(t_ambiant *ambiant);
int			set_camera(t_camera *camera);

int			register_type(char *id, void *(*parser)(char **),
				void (*render)(t_minirt *, t_ray *, t_object *),
				void (*updater)(t_minirt *, t_object *));
int			exist_type(char *id);
void		*get_parser_by_id(char *id);
void		*get_render_by_id(char *id);
void		*get_updater_by_id(char *id);

void		update_values(t_minirt *mrt);
void		update_object_colors(t_minirt *mrt, t_object *object);

//parsing
int			parse_map(char *path);
int			parse_fvector3(char *value, t_fvector3 *v3,
				char *invalid_format_error);
int			parse_normal(char *value, t_fvector3 *normal,
				char *invalid_format_error);
int			parse_color(char *value, t_rgb *rgb, char *invalid_format_error);
void		*error_and_null(char *error);

#endif
