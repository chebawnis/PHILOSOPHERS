/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adichou <adichou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 22:22:36 by adichou           #+#    #+#             */
/*   Updated: 2025/09/14 23:45:17 by adichou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	take_fork(t_philosopher *ps, pthread_mutex_t *fork)
{
	pthread_mutex_lock(fork);
	pthread_mutex_lock(&(ps->program->printf_mutex));
	if (!ps->program->should_program_stop)
		printf("%ld %d has taken a fork\n", get_p_time(ps), ps->id);
	pthread_mutex_unlock(&(ps->program->printf_mutex));
}
