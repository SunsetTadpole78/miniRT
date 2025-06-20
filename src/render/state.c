/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroussel <lroussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 07:20:49 by lroussel          #+#    #+#             */
/*   Updated: 2025/06/20 11:56:40 by lroussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	can_exit(t_minirt *mrt)
{
	int	v;

	pthread_mutex_lock(&mrt->exit_mutex);
	v = mrt->exit;
	pthread_mutex_unlock(&mrt->exit_mutex);
	return (v);
}

void	stop_threads(t_minirt *mrt)
{
	int	i;

	pthread_mutex_lock(&mrt->exit_mutex);
	mrt->exit = 1;
	pthread_mutex_unlock(&mrt->exit_mutex);
	i = 0;
	while (i < (mrt->cores - 1))
	{
		sem_post(&mrt->threads_datas[i].update_sem);
		pthread_join(mrt->threads_datas[i].thread, NULL);
		i++;
	}
}
