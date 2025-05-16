/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elements.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 03:08:17 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/16 03:08:26 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

int	parse_fvector3(char *value, t_fvector3 *v3,
			char *invalid_format_error)
{
	char	**splited;
	int		valid;

	splited = ft_split(value, ',');
	if (!splited[0] || !splited[1] || !splited[2] || splited[3]
		|| !ft_isnumeric(splited[0]) || !ft_isnumeric(splited[1])
		|| !ft_isnumeric(splited[2]) || ft_isoutint(splited[0])
		|| ft_isoutint(splited[1]) || ft_isoutint(splited[2])
	)
		valid = ft_error(invalid_format_error, ERR_PREFIX, 0);
	else
	{
		*v3 = ft_fvector3(ft_atof(splited[0]), ft_atof(splited[1]),
				ft_atof(splited[2]));
		valid = 1;
	}
	ft_free_str_array(splited);
	return (valid);
}

int	parse_normal(char *value, t_fvector3 *normal, char *invalid_format_error)
{
	if (!parse_fvector3(value, normal, invalid_format_error))
		return (0);
	if (normal->x < -1.0f || normal->x > 1.0f || normal->y < -1.0f
		|| normal->y > 1.0f || normal->z < -1.0f || normal->z > 1.0f)
	{
		ft_error(invalid_format_error, ERR_PREFIX, 0);
		return (0);
	}
	return (1);
}

int	parse_color(char *value, t_rgb *color, char *invalid_format_error)
{
	*color = ft_atorgb(value);
	if (color->r == -1)
		return (ft_error(invalid_format_error, ERR_PREFIX, 0));
	return (1);
}
