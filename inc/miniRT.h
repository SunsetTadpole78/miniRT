/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:30:37 by lroussel          #+#    #+#             */
/*   Updated: 2025/04/23 14:05:13 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "libft.h"
# include "mlx.h"

# define AMBIANT_ID "A"
# define CAMERA_ID "C"
# define CYLINDER_ID "cl"
# define LIGHT_ID "L"
# define PLANE_ID "pl"
# define SPHERE_ID "sp"

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

typedef struct s_minirt
{
	t_list		*objects;
	t_list		*lights;
	t_ambiant	*ambiant;
	t_camera	*camera;
}	t_minirt;

t_minirt	*minirt(void);
t_minirt	*init_minirt(void);
void		destruct_minirt(void);

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
