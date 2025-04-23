/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:31:04 by lroussel          #+#    #+#             */
/*   Updated: 2025/04/23 11:49:34 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	destruct_minirt(void)
{
	t_minirt	*mrt;

	mrt = minirt();
	ft_lstclear(&mrt->objects, free);
	free(mrt);
}
