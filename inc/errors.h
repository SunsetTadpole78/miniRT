/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:59:42 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/26 11:01:59 by lroussel         ###   ########.fr       */
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
# define L_ARGS_E "Light format: 'l <position> <level> <color> [scale]'\n"
# define L_POS_E "Light position format: [INT_MIN;INT_MAX],[INT_MIN;INT_MAX]\
,[INT_MIN;INT_MAX]\n"
# define L_LVL_E "Light level format: [0.0;1.0]\n"
# define L_SCALE_E "Light scale format: [0.0;INT_MAX]"
# define L_RGB_E "Light rgb format: [0;255],[0;255],[0;255]\n"
# define PL_ARGS_E "Plane format: 'pl <position> <normal> <rgb|<texture|texture,bump>> [<pattern>\
 [color2]] [<smoothness> [mattifying]]'\n"
# define PL_POS_E "Plane position format: [INT_MIN;INT_MAX],[INT_MIN;INT_MAX]\
,[INT_MIN;INT_MAX]\n"
# define PL_NORM_E "Plane normal format: [-1.0;1.0],[-1.0;1.0],[-1.0;1.0]\n"
# define PL_TEXTURE_E "Plane texture format: [0;255],[0;255],[0;255]\
, path.xpm or path.xpm,bump.xpm\n"
# define SP_ARGS_E "Sphere format: 'sp <position> <diameter> <rgb|<texture|texture,bump>> [<pattern>\
 [color2]] [<smoothness> [mattifying]]'\n"
# define SP_POS_E "Sphere position format: [INT_MIN;INT_MAX],[INT_MIN;INT_MAX]\
,[INT_MIN;INT_MAX]\n"
# define SP_DIAM_E "Sphere diameter format: [0.0;INT_MAX]\n"
# define SP_TEXTURE_E "Sphere texture format: [0;255],[0;255],[0;255]\
, path.xpm or path.xpm,bump.xpm\n"
# define CY_ARGS_E "Cylinder format: 'cy <position> <normal> <diameter> <height> \
<rgb|<texture|texture,bump>> [<pattern> [color2]] [<smoothness> [mattifying]]'\n"
# define CY_POS_E "Cylinder position format: [INT_MIN;INT_MAX],[INT_MIN;INT_MAX]\
,[INT_MIN;INT_MAX]\n"
# define CY_NORM_E "Cylinder normal format: [-1.0;1.0],[-1.0;1.0],[-1.0;1.0]\n"
# define CY_DIAM_E "Cylinder diameter format: [0.0;INT_MAX]\n"
# define CY_HEI_E "Cylinder height format: [0.0;INT_MAX|-1]\n"
# define CY_TEXTURE_E "Cylinder texture format: [0;255],[0;255],[0;255]\
, path.xpm or path.xpm,bump.xpm\n"
# define CO_ARGS_E "Cone format: 'co <position> <normal> <base_diameter> <height> \
<rgb|<texture|texture,bump>> [<pattern> [color2]] [<smoothness> [mattifying]]'\n"
# define CO_POS_E "Cone position format: [INT_MIN;INT_MAX],[INT_MIN;INT_MAX]\
,[INT_MIN;INT_MAX]\n"
# define CO_NORM_E "Cone normal format: [-1.0;1.0],[-1.0;1.0],[-1.0;1.0]\n"
# define CO_DIAM_E "Cone base diameter format: [0.0;INT_MAX]\n"
# define CO_HEI_E "Cone height format: [0.0;INT_MAX|-1]\n"
# define CO_TEXTURE_E "Cone texture format: [0;255],[0;255],[0;255]\
, path.xpm or path.xpm,bump.xpm\n"

#endif
