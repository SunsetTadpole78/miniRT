/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:34:33 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/02 13:37:44 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

void	*error_and_null(char *error)
{
	ft_error(error, ERR_PREFIX, 0);
	return (NULL);
}

void	init_pattern(t_pattern *pattern)
{
	pattern->id = '\0';
	pattern->main_color = (t_rgb){0, 0, 0};
	pattern->secondary_color = (t_rgb){0, 0, 0};
	pattern->smoothness = 1.0f;
	pattern->smoothness_factor = 0.0f;
	pattern->mattifying = 0.0f;
	pattern->path = NULL;
	pattern->texture.ptr = NULL;
	pattern->texture.data = NULL;
	pattern->texture.bpp = 0;
	pattern->texture.ll = 0;
	pattern->texture.cl = 0;
	pattern->texture.endian = 0;
	pattern->texture.height = 0;
	pattern->texture.width = 0;
}
