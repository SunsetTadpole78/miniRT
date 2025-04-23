/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:30:37 by lroussel          #+#    #+#             */
/*   Updated: 2025/04/23 11:34:16 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "libft.h"
# include "mlx.h"

typedef struct s_object
{
	char	*id;
//	t_vector3	position;
//	t_vector3	rotation;
//	t_rgb		rgb;
}	t_object;

typedef struct s_minirt
{
	t_list	*objects;
}	t_minirt;

t_minirt	*minirt(void);
t_minirt	*init_minirt(void);
void		destruct_minirt(void);

#endif
