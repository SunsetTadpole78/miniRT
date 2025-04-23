/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:49:15 by lroussel          #+#    #+#             */
/*   Updated: 2025/04/23 11:30:44 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* -------------------------------- PROTOTYPE ------------------------------- */
static t_minirt	*glob_minirt(t_minirt *mrt, int replace);
/* -------------------------------------------------------------------------- */

t_minirt	*init_minirt(void)
{
	t_minirt	*mrt;

	mrt = malloc(sizeof(t_minirt));
	if (!mrt)
		return (NULL);
	mrt->objects = NULL;
	glob_minirt(mrt, 1);
	return (mrt);
}

t_minirt	*minirt(void)
{
	return (glob_minirt(NULL, 0));
}

static t_minirt	*glob_minirt(t_minirt *mrt, int replace)
{
	static t_minirt	*content = NULL;

	if (replace)
		content = mrt;
	return (content);
}
