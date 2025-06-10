/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:29:54 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/03 02:50:54 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	parse_texture(char *value, t_pattern *pattern)
{
	int	len;

	if (parse_color(value, &pattern->main_color))
		return (1);
	len = ft_strlen(value);
	if (len < 4 || ft_strncmp(value + (len - 4), ".xpm", 5) != 0)
		return (0);
	if (access(value, R_OK) != 0)
		return (0);
	pattern->path = ft_strdup(value);
	pattern->main_color = (t_rgb){0, 0, 0};
	return (1);
}

int	parse_reflection(char **values, t_pattern *pattern)
{
	if (!ft_isnumeric(values[0]))
		return (0);
	pattern->smoothness = ft_atof(values[0]);
	pattern->smoothness_factor = 1.0f - cosf(RAD_ANGLE
			* (1.0f - pattern->smoothness));
	if (!values[1])
		return (1);
	if (ft_isnumeric(values[1]))
	{
		pattern->mattifying = ft_atof(values[1]);
		return (2);
	}
	return (1);
}

int	parse_pattern(char **values, t_pattern *pattern)
{
	int	res;
	int	parsed;

	res = parse_reflection(values, pattern);
	parsed = res;
	if (!values[parsed])
		return (1);
	if (ft_strncmp(values[parsed], "checker", 8) != 0)
		return (0);
	pattern->id = 'c';
	parsed++;
	if (values[parsed])
	{
		parsed += parse_color(values[parsed], &pattern->secondary_color);
		if (!values[parsed])
			return (1);
		if (res != 0)
			return (0);
		res = parse_reflection(values + parsed, pattern);
		if (res == 0 || values[parsed + res])
			return (0);
	}
	return (1);
}
