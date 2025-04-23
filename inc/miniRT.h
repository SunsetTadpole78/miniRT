/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:30:37 by lroussel          #+#    #+#             */
/*   Updated: 2025/04/23 12:18:35 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "libft.h"
# include "mlx.h"

typedef struct s_object
{
	char		*id;
	t_fvector3	position;
	t_fvector3	rotation;
	t_rgb		rgb;
}	t_object;

typedef struct s_minirt
{
	t_list	*objects;
}	t_minirt;

t_minirt	*minirt(void);
t_minirt	*init_minirt(void);
void		destruct_minirt(void);

//objects
void		register_object(char *id, t_object *object);
t_object	*create_object(t_fvector3 position, t_fvector3 rotation, t_rgb rgb);

#endif
