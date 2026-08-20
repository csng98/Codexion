/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csekakul <csekakul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 09:05:56 by csekakul          #+#    #+#             */
/*   Updated: 2026/08/20 11:17:05 by csekakul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	coder_compile(t_coder *coder)
{
	acquire_dongles(coder);
	pthread_mutex_lock(&coder->data->state_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->data->state_mutex);
	safe_print(coder->data, coder->id, "is compiling");
	precise_sleep(coder->data->time_to_compile, coder->data);
	release_dongles(coder);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->id % 2 == 0)
		usleep(1000);
	while (!is_simulation_over(coder->data))
	{
		coder_compile(coder);
		if (is_simulation_over(coder->data))
			break ;
		safe_print(coder->data, coder->id, "is debugging");
		precise_sleep(coder->data->time_to_debug, coder->data);
		if (is_simulation_over(coder->data))
			break ;
		safe_print(coder->data, coder->id, "is refactoring");
		precise_sleep(coder->data->time_to_refactor, coder->data);
		pthread_mutex_lock(&coder->data->state_mutex);
		coder->compiles_count++;
		pthread_mutex_unlock(&coder->data->state_mutex);
	}
	return (NULL);
}
