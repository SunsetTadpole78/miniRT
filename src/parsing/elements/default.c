/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 03:08:17 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 13:37:33 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

int	parse_fvector3(char *value, t_fvector3 *v3)
{
	char	**splited;
	int		valid;

	if (value[ft_strlen(value) - 1] == ',')
		return (0);
	splited = ft_split(value, ',');
	if (!splited[0] || !splited[1] || !splited[2] || splited[3]
		|| !ft_isnumeric(splited[0]) || !ft_isnumeric(splited[1])
		|| !ft_isnumeric(splited[2]) || ft_isoutint(splited[0])
		|| ft_isoutint(splited[1]) || ft_isoutint(splited[2])
	)
		valid = 0;
	else
	{
		*v3 = (t_fvector3){ft_atof(splited[0]), ft_atof(splited[1]),
			ft_atof(splited[2])};
		valid = 1;
	}
	ft_free_str_array(splited);
	return (valid);
}

int	parse_normal(char *value, t_fvector3 *normal)
{
	if (!parse_fvector3(value, normal))
		return (0);
	return (normal->x >= -1.0f && normal->x <= 1.0f
		&& normal->y >= -1.0f && normal->y <= 1.0f
		&& normal->z >= -1.0f && normal->z <= 1.0f);
}

int	parse_color(char *value, t_rgb *color)
{
	*color = ft_atorgb(value);
	return (color->r != -1);
}
