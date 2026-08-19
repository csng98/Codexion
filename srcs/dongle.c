/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csekakul <csekakul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 09:06:01 by csekakul          #+#    #+#             */
/*   Updated: 2026/08/19 12:07:36 by csekakul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongles(t_data *data)
{
	int	i;

	data->dongles = malloc(sizeof(t_dongle) * data->num_coders);
	if (!data->dongles)
		return (0);
	i = 0;
	while (i < data->num_coders)
	{
		data->dongles[i].id = i + 1;
		data->dongles[i].is_in_use = 0;
		data->dongles[i].available_at = 0;
		if (pthread_mutex_init(&data->dongles[i].mutex, NULL) != 0)
			return (0);
		if (pthread_cond_init(&data->dongles[i].cond, NULL) != 0)
			return (0);
		if (!heap_init(&data->dongles[i].queue, data->num_coders))
			return (0);
		i++;
	}
	return (1);
}

t_request	heap_pop(t_heap *heap)
{
	t_request	top;
	t_request	empty;

	empty.coder_id = -1;
	empty.key = -1;
	if (heap->size <= 0)
		return (empty);
	top = heap->data[0];
	heap->data[0] = heap->data[heap->size - 1];
	heap->size--;
	return (top);
}

static void	grab_single_dongle(t_coder *coder, t_dongle *dongle)
{
	long long	key;
	t_request	top;

	if (coder->data->scheduler == SCHED_FIFO)
		key = get_time_ms();
	else
		key = coder->last_compile_start + coder->data->time_to_burnout;
	pthread_mutex_lock(&dongle->mutex);
	heap_push(&dongle->queue, coder->id, key);
	while (!is_simulation_over(coder->data))
	{
		top = heap_peek(&dongle->queue);
		if (top.coder_id == coder->id && !dongle->is_in_use
			&& get_time_ms() >= dongle->available_at)
			break ;
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	heap_pop(&dongle->queue);
	dongle->is_in_use = 1;
	pthread_mutex_unlock(&dongle->mutex);
	safe_print(coder->data, coder->id, "has taken a dongle");
}


