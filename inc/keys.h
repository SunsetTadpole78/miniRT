/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 19:14:13 by lroussel          #+#    #+#             */
/*   Updated: 2025/05/17 20:45:26 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_H
# define KEYS_H

/*
 *	OPENGL KEYS
 */

# define OGLK_ESC 53

# define OGLK_FOVP 24
# define OGLK_FOVM 27

// camera movements
# define OGLK_FORWARD 13
# define OGLK_BACKWARD 1
# define OGLK_LEFT 0
# define OGLK_RIGHT 2
# define OGLK_SPACE 49
# define OGLK_SHIFT 257

// camera rotation
# define OGLK_ARROW_UP 126
# define OGLK_ARROW_DOWN 125
# define OGLK_ARROW_LEFT 123
# define OGLK_ARROW_RIGHT 124

// start high definition
# define OGLK_R 15

/*
 *	X11 KEYS
 */

# define XK_ESC 65307

# define XK_FOVP '='
# define XK_FOVM '-'

// camera movements
# define XK_FORWARD 'w'
# define XK_BACKWARD 's'
# define XK_LEFT 'a'
# define XK_RIGHT 'd'
# define XK_SPACE ' '
# define XK_SHIFT 65505

// camera rotation
# define XK_ARROW_UP 65362
# define XK_ARROW_DOWN 65364
# define XK_ARROW_LEFT 65361
# define XK_ARROW_RIGHT 65363

// start high definition
# define XK_R -1

#endif
