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

// Standard Libraries

# include <stdio.h>

// Local Libraries

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

# define WIN_HEIGHT 1100
# define WIN_WIDTH 900

// Pour Macos: 53 ; Pour Linux: 65307
# define MAIN_PAD_ESC 53

typedef struct s_ambiant
{
	char		*id;
	float		level;
	t_rgb		color;
}	t_ambiant;

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
}	t_mlx;

typedef struct s_minirt
{
	t_list		*objects;
	t_list		*lights;
	t_ambiant	*ambiant;
	t_camera	*camera;
	t_mlx		*mlx;
}	t_minirt;

t_minirt	*minirt(void);
void		destruct_minirt(t_minirt *mrt);

// mlx
void		init_mlx(t_mlx *mlx);

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
