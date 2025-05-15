/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:30:37 by lroussel          #+#    #+#             */
/*   Updated: 2025/04/23 15:35:12 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

// Libraries

# include <stdio.h>
# include <math.h>

# include "libft.h"
# include "mlx.h"

// Macros for Type Objects

# define AMBIANT_ID "A"
# define CAMERA_ID "C"
# define CYLINDER_ID "cl"
# define LIGHT_ID "L"
# define PLANE_ID "pl"
# define SPHERE_ID "sp"

// Macros for MLX

# define WIN_HEIGHT 700
# define WIN_WIDTH 1100

# define MAC_ESC_KEY 53
# define LINUX_ESC_KEY 65307

// Structures

typedef struct s_ambiant
{
	char		*id;
	float		level;
	t_rgb		color;
}	t_ambiant;

typedef struct s_ray
{
	t_fvector3		origin;
	t_fvector3		direction;
}	t_ray;

typedef struct s_camera
{
	char		*id;
	t_fvector3	position;
	t_fvector3	normal;
	int			fov;
}	t_camera;

typedef struct s_light
{
	char		*id;
	t_fvector3	position;
	float		level;
	t_rgb		color;
}	t_light;

typedef struct s_sphere
{
	char		*id;
	t_fvector3	position;
	float		diameter;
	t_rgb		color;
}	t_sphere;

typedef struct s_plane
{
	char		*id;
	t_fvector3	position;
	t_fvector3	normal;
	t_rgb		color;
}	t_plane;

typedef struct s_cylinder
{
	char		*id;
	t_fvector3	position;
	t_fvector3	normal;
	t_fvector2	size;
	t_rgb		color;
}	t_cylinder;

typedef struct s_identifiable
{
	char	*id;
}	t_identifiable;

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

typedef struct s_minirt
{
	t_list		*objects;
	t_list		*lights;
	t_ambiant	*ambiant;
	t_camera	*camera;
	t_plane		*plane;
	t_mlx		*mlx;
}	t_minirt;

t_minirt	*minirt(void);
void		destruct_minirt(t_minirt *mrt);
void		render_scene(t_minirt *mrt);

void		init_mlx(t_mlx *mlx);
void		init_cam_and_plane(void);

//func_math_fvector3.c
t_fvector3	normalize(t_fvector3 *v);
t_fvector3	cross(t_fvector3 *v1, t_fvector3 *v2);
t_fvector3	add_vectors(t_fvector3 *v1, t_fvector3 *v2);
t_fvector3	sub_vectors(t_fvector3 *v1, t_fvector3 *v2);
float		dot(t_fvector3 *v1, t_fvector3 *v2);

//objects
t_ambiant	*ambiant(float level, t_rgb color);
t_camera	*camera(t_fvector3 position, t_fvector3 normal, int fov);
t_cylinder	*cylinder(t_fvector3 position, t_fvector3 normal,
				t_fvector2 size, t_rgb color);
int			register_object(void *object);
int			register_light(t_light *light);
int			set_ambiant(t_ambiant *ambiant);
int			set_camera(t_camera *camera);
t_light		*light(t_fvector3 position, float level, t_rgb color);
t_plane		*plane(t_fvector3 position, t_fvector3 normal, t_rgb color);
t_sphere	*sphere(t_fvector3 position, float diameter, t_rgb color);

#endif
