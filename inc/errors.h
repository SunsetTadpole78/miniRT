/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:59:42 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/14 18:22:51 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# define RED "\033[91m"

# define USAGE_ERROR "miniRT: Usage: /miniRT <map_path>\n"
# define MALLOC_ERROR "miniRT: Malloc Error.\n"
# define MAP_NOT_FOUND_ERROR "miniRT: Map path not found\n"
# define INVALID_OBJECT_ERROR "miniRT: Invalid object id\n"
# define INVALID_AMBIANT_ARGS_ERROR "miniRT: Ambiant format: 'A <level> <color>'\n"
# define INVALID_AMBIANT_LEVEL_ERROR "miniRT: Ambiant level format: [0.0;1.0]\n"

#endif
