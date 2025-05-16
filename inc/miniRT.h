/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:30:37 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/16 13:53:40 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

// Libraries

# include <fcntl.h>
# include <stdio.h>

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

# define OPENGL_ESC_KEY 53
# define X11_ESC_KEY 65307

// Structures

typedef struct s_object
{
	char			*id;
	struct s_object	*next;
}	t_object;

typedef struct s_type
{
	char			*id;
	void			*(*parser)(char **);
	void			(*render)(t_object *);
	struct s_type	*next;
}	t_type;

typedef struct s_ambiant
{
	char		*id;
	t_object	*next;
	float		level;
	t_rgb		color;
}	t_ambiant;

typedef struct s_camera
{
	char		*id;
	t_object	*next;
	t_fvector3	position;
	t_fvector3	normal;
	int			fov;
	float			norm_fov;
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
}	t_mlx;

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

void		init_mlx(t_mlx *mlx);
void		destruct_mlx(t_mlx *mlx);

void		render_scene(t_minirt *mrt);

void		init_cam_and_plane(void);

//func_math_fvector3.c
t_fvector3	normalize(t_fvector3 v);
t_fvector3	cross(t_fvector3 v1, t_fvector3 v2);
t_fvector3	add_vectors(t_fvector3 v1, t_fvector3 v2);
t_fvector3	sub_vectors(t_fvector3 v1, t_fvector3 v2);
float		dot(t_fvector3 v1, t_fvector3 v2);

//objects
t_ambiant	*ambiant(float level, t_rgb color);
void		*parse_ambiant(char **values);

t_camera	*camera(t_fvector3 position, t_fvector3 normal, int fov);
void		*parse_camera(char **values);

t_cylinder	*cylinder(t_fvector3 position, t_fvector3 normal,
				t_fvector2 size, t_rgb color);
void		*parse_cylinder(char **values);

t_light		*light(t_fvector3 position, float level, t_rgb color);
void		*parse_light(char **values);

t_plane		*plane(t_fvector3 position, t_fvector3 normal, t_rgb color);
void		*parse_plane(char **values);

t_sphere	*sphere(t_fvector3 position, float diameter, t_rgb color);
void		*parse_sphere(char **values);

int			register_object(void *object);
int			register_light(t_light *light);
int			set_ambiant(t_ambiant *ambiant);
int			set_camera(t_camera *camera);

int			register_type(char *id, void *(*parser)(char **),
				void (*render)(t_object *));
int			exist_type(char *id);
void		*get_parser_by_id(char *id);

//parsing
int			parse_map(char *path);
int			parse_fvector3(char *value, t_fvector3 *v3,
				char *invalid_format_error);
int			parse_normal(char *value, t_fvector3 *normal,
				char *invalid_format_error);
int			parse_color(char *value, t_rgb *rgb, char *invalid_format_error);
void		*error_and_null(char *error);

#endif
