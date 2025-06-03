/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 07:20:49 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/03 07:21:35 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	on_finish(t_minirt *mrt)
{
	pthread_mutex_lock(&mrt->workers_mutex);
	mrt->workers--;
	pthread_mutex_unlock(&mrt->workers_mutex);
}

int	is_update(t_minirt *mrt, t_thread_data *data)
{
	int	v;

	pthread_mutex_lock(&mrt->update_mutex);
	v = data->update;
	pthread_mutex_unlock(&mrt->update_mutex);
	return (v);
}

int	is_working(t_minirt *mrt)
{
	int	v;

	pthread_mutex_lock(&mrt->workers_mutex);
	v = mrt->workers;
	pthread_mutex_unlock(&mrt->workers_mutex);
	return (v);
}
