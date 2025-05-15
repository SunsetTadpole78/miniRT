/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:59:42 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/15 10:29:55 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# define ERR_PREFIX "\033[91mminiRT: "

# define USAGE_ERR "Usage: /miniRT <map_path>\n"
# define MALLOC_ERR "Malloc Error.\n"
# define MAP_NOT_FOUND_ERR "Map path not found\n"
# define INV_OBJECT_ERR "Invalid object id\n"
# define INV_AMBIANT_ARGS_ERR "Ambiant format: 'A <level> <color>'\n"
# define INV_AMBIANT_LEVEL_ERR "Ambiant level format: [0.0;1.0]\n"
# define INV_AMBIANT_RGB_ERR "Ambiant rgb format: [0;255],[0;255],[0;255]\n"
# define REGISTRATION_ERR "Object Registration Error\n"

#endif
