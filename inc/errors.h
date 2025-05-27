/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:59:42 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/27 21:26:29 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# define ERR_PREFIX "Error:\n\033[91m» \033[1;93mminiRT\033[91m: "

# define USAGE_E "Usage: /miniRT <map_path>\n"
# define MALLOC_E "Malloc Error.\n"
# define CORES_E "Failed to retrieve cores count\n"
# define EXTENSION_E "Map file must be respect format: *.rt\n"
# define REGISTRATION_E "Object Registration Error\n"
# define MAP_NOT_FOUND_E "Map path not found\n"
# define NEED_CAMERA_E "Camera element needed\n"
# define OBJ_E "Invalid object id\n"
# define UNOPENED_COM_E "Unopened comment\n"
# define UNCLOSED_COM_E "Unclosed comment\n"
# define A_ARGS_E "Ambiant format: 'A <level> <color>'\n"
# define A_LVL_E "Ambiant level format: [0.0;1.0]\n"
# define A_RGB_E "Ambiant rgb format: [0;255],[0;255],[0;255]\n"
# define C_ARGS_E "Camera format: 'C <position> <normal> <fov>'\n"
# define C_POS_E "Camera position format: [INT_MIN;INT_MAX],[INT_MIN;INT_MAX]\
,[INT_MIN;INT_MAX]\n"
# define C_NORM_E "Camera normal format: [-1.0;1.0],[-1.0;1.0],[-1.0;1.0]\n"
# define C_FOV_E "Camera fov format: [0;180]\n"
# define L_ARGS_E "Camera format: 'L <position> <level> <color>'\n"
# define L_POS_E "Light position format: [INT_MIN;INT_MAX],[INT_MIN;INT_MAX]\
,[INT_MIN;INT_MAX]\n"
# define L_LVL_E "Light level format: [0.0;1.0]\n"
# define L_RGB_E "Light rgb format: [0;255],[0;255],[0;255]\n"
# define PL_ARGS_E "Plane format: 'pl <position> <normal> <color>'\n"
# define PL_POS_E "Plane position format: [INT_MIN;INT_MAX],[INT_MIN;INT_MAX]\
,[INT_MIN;INT_MAX]\n"
# define PL_NORM_E "Plane normal format: [-1.0;1.0],[-1.0;1.0],[-1.0;1.0]\n"
# define PL_RGB_E "Plane rgb format: [0;255],[0;255],[0;255]\n"
# define SP_ARGS_E "Sphere format: 'sp <position> <diameter> <color> [<pattern>\
 [color2]] [<smoothness> [mattifying]]'\n"
# define SP_POS_E "Sphere position format: [INT_MIN;INT_MAX],[INT_MIN;INT_MAX]\
,[INT_MIN;INT_MAX]\n"
# define SP_DIAM_E "Sphere diameter format: [0.0;INT_MAX]\n"
# define SP_RGB_E "Sphere rgb format: [0;255],[0;255],[0;255]\n"
# define CY_ARGS_E "Cylinder format: 'cy <position> <normal> <diameter> <height> \
<color>'\n"
# define CY_POS_E "Cylinder position format: [INT_MIN;INT_MAX],[INT_MIN;INT_MAX]\
,[INT_MIN;INT_MAX]\n"
# define CY_NORM_E "Cylinder normal format: [-1.0;1.0],[-1.0;1.0],[-1.0;1.0]\n"
# define CY_DIAM_E "Cylinder diameter format: [0.0;INT_MAX]\n"
# define CY_HEI_E "Cylinder height format: [0.0;INT_MAX]\n"
# define CY_RGB_E "Cylinder rgb format: [0;255],[0;255],[0;255]\n"

#endif
