/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambiant.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:58:48 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/14 18:20:43 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_ambiant	*ambiant(float level, t_rgb color)
{
	t_ambiant	*a;

	a = malloc(sizeof(t_ambiant));
	if (!a)
		return (NULL);
	a->id = AMBIANT_ID;
	a->level = level;
	a->color = color;
	return (a);
}

void	*parse_ambiant(char **values)
{
	float	level;

	if (!values[0] || !values[1] || values[2])
	{
		ft_error(INVALID_AMBIANT_ARGS_ERROR, RED, 0);
		return (NULL);
	}
	if (!ft_isfloat(values[0]))
	{
		ft_error(INVALID_AMBIANT_LEVEL_ERROR, RED, 0);
		return (NULL);
	}
	level = ft_atof(values[0]);
	if (level < 0.0f || level > 1.0f)
	{
		ft_error(INVALID_AMBIANT_LEVEL_ERROR, RED, 0);
		return (NULL);
	}
	return (NULL);
}
