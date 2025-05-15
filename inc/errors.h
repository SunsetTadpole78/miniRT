/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:59:42 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/16 01:14:09 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# define ERR_PREFIX "\033[91mminiRT: "

# define USAGE_E "Usage: /miniRT <map_path>\n"
# define MALLOC_E "Malloc Error.\n"
# define REGISTRATION_E "Object Registration Error\n"
# define MAP_NOT_FOUND_E "Map path not found\n"
# define INV_OBJ_E "Invalid object id\n"
# define INV_A_ARGS_E "Ambiant format: 'A <level> <color>'\n"
# define INV_A_LVL_E "Ambiant level format: [0.0;1.0]\n"
# define INV_A_RGB_E "Ambiant rgb format: [0;255],[0;255],[0;255]\n"
# define INV_C_ARGS_E "Camera format: 'A <position> <normal> <fov>'\n"
# define INV_C_POS_E "Camera position format: <f>,<f>,<f>\n"
# define INV_C_NORM_E "Camera normal format: [-1.0;1.0],[-1.0;1.0],[-1.0;1.0]\n"
# define INV_C_FOV_E "Camera fov format: [0;180]\n"

#endif
