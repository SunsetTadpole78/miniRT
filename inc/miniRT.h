/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:30:37 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/17 20:53:06 by lroussel         ###   ########.fr       */
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

typedef struct s_fmatrix
{
	float		m[4][4];
}	t_fmatrix;

typedef struct s_object
{
	char			*id;
	struct s_object	*next;
}	t_object;

typedef struct s_ambiant
{
	char		*id;
	t_object	*next;
	float		level;
	t_rgb		color;
}	t_ambiant;

typedef struct s_ray
{
	t_fvector3		origin;
	t_fvector3		direction;
	float			dist;
}	t_ray;

// position dans l'espace monde.
// normal = le regarde de la caméra.
// right: vecteur horizontal
// up: vecteur vertical
typedef struct s_camera
{
	char		*id;
	t_object	*next;
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
	t_fvector3	position;
	float		level;
	t_rgb		color;
}	t_light;

typedef struct s_sphere
{
	char		*id;
	t_object	*next;
	t_fvector3	position;
	float		diameter;
	float		radius;
	t_rgb		color;
}	t_sphere;

typedef struct s_plane
{
	char		*id;
	t_object	*next;
	t_fvector3	position;
	t_fvector3	normal;
	t_rgb		color;
}	t_plane;

typedef struct s_cylinder
{
	char		*id;
	t_object	*next;
	t_fvector3	position;
	t_fvector3	normal;
	t_fvector2	size;
	t_rgb		color;
}	t_cylinder;

typedef struct s_mlx
{
	void		*mlx_ptr;
	void		*win_ptr;
	void		*img_ptr;
	char		*data;
	int			bpp;
	int			size_line;
	int			endian;
}	t_mlx;

typedef struct s_type
{
	char			*id;
	void			*(*parser)(char **);
	void			(*render)(t_mlx *, t_ray *, t_fvector2, t_object *);
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

// vector
t_fvector3	ft_fvector3_scale(t_fvector3 v, float s);

//objects
t_ambiant	*ambiant(float level, t_rgb color);
void		*parse_ambiant(char **values);

t_camera	*camera(t_fvector3 position, t_fvector3 normal, int fov);
void		*parse_camera(char **values);
void		rotate_camera_y(t_camera *cam, float theta);
void		add_fov(t_minirt *mrt, int incrementation);
void		move(t_minirt *mrt, float *coordinate, float incrementation);

t_cylinder	*cylinder(t_fvector3 position, t_fvector3 normal,
				t_fvector2 size, t_rgb color);
void		*parse_cylinder(char **values);

t_light		*light(t_fvector3 position, float level, t_rgb color);
void		*parse_light(char **values);

t_plane		*plane(t_fvector3 position, t_fvector3 normal, t_rgb color);
void		*parse_plane(char **values);
void		render_plane(t_mlx *mlx, t_ray *ray,
				t_fvector2 pixel, t_object *object);

t_sphere	*sphere(t_fvector3 position, float diameter, t_rgb color);
void		*parse_sphere(char **values);
void		render_sphere(t_mlx *mlx, t_ray *ray,
				t_fvector2 pixel, t_object *object);

int			register_object(void *object);
int			register_light(t_light *light);
int			set_ambiant(t_ambiant *ambiant);
int			set_camera(t_camera *camera);

int			register_type(char *id, void *(*parser)(char **),
				void (*render)(t_mlx *, t_ray *, t_fvector2, t_object *));
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

//TESTS
void		put_pixel(t_mlx *mlx, t_fvector2 v, t_rgb rgb);

#endif
