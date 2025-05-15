/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambiant.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:58:48 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/15 10:30:43 by lroussel         ###   ########.fr       */
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
	t_rgb	rgb;

	if (!values[0] || !values[1] || values[2])
	{
		ft_error(INV_AMBIANT_ARGS_ERR, ERR_PREFIX, 0);
		return (NULL);
	}
	level = ft_atof(values[0]);
	if (!ft_isfloat(values[0]) || level < 0.0f || level > 1.0f)
	{
		ft_error(INV_AMBIANT_LEVEL_ERR, ERR_PREFIX, 0);
		return (NULL);
	}
	rgb = ft_atorgb(values[1]);
	if (rgb.r == -1)
	{
		ft_error(INV_AMBIANT_RGB_ERR, ERR_PREFIX, 0);
		return (NULL);
	}
	return (ambiant(level, rgb));
}
