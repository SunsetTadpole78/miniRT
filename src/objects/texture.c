/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:   by Juste                               #+#    #+#             */
/*   Updated:   by Juste                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "errors.h"

t_texture	init_texture(char *pathname, void *mlx_ptr)
{
	t_texture	tex;

	tex.image = mlx_xpm_file_to_image(mlx_ptr, pathname,
			&tex.width, &tex.height);
	if (!tex.image)
		return ((t_texture){NULL, NULL, 0, 0, 0, 0, 0, 0});
	tex.data = mlx_get_data_addr(tex.image, &tex.bpp, &tex.ll, &tex.endian);
	tex.cl = tex.bpp / 8;
	return (tex);
}
