/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:34:33 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/26 11:21:50 by lroussel         ###   ########.fr       */
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
	pattern->bump_path = NULL;
	pattern->bump.ptr = NULL;
	pattern->bump.data = NULL;
	pattern->bump.bpp = 0;
	pattern->bump.ll = 0;
	pattern->bump.cl = 0;
	pattern->bump.endian = 0;
	pattern->bump.height = 0;
	pattern->bump.width = 0;
}

void	init_texture(t_mlx_image *texture, char *path, void *ptr)
{
	texture->ptr = mlx_xpm_file_to_image(ptr, path, &texture->width,
			&texture->height);
	texture->data = mlx_get_data_addr(texture->ptr, &texture->bpp, &texture->ll,
			&texture->endian);
	texture->cl = texture->bpp / 8;
	texture->ratio = (float)texture->width / (float)texture->height;
}
