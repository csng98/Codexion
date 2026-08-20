/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csekakul <csekakul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 09:06:10 by csekakul          #+#    #+#             */
/*   Updated: 2026/08/20 10:25:17 by csekakul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_data *data, int i, long long now)
{
	long long	diff;

	diff = now - data->coders[i].last_compile_start;
	if (diff > data->time_to_burnout)
	{
		data->simulation_end = 1;
		pthread_mutex_lock(&data->log_mutex);
		printf("%lld %d burned out\n",
			now - data->start_time, data->coders[i].id);
		pthread_mutex_unlock(&data->log_mutex);
		pthread_mutex_unlock(&data->state_mutex);
		return (1);
	}
	return (0);
}

static int	check_all_coders(t_data *data, int *all_finished, long long now)
{
	int	i;

	i = 0;
	while (i < data->num_coders)
	{
		if (check_burnout(data, i, now))
			return (1);
		if (data->compiles_required == -1
			|| data->coders[i].compiles_count < data->compiles_required)
			*all_finished = 0;
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data		*data;
	int			all_finished;
	long long	now;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->state_mutex);
		now = get_time_ms();
		all_finished = 1;
		if (check_all_coders(data, &all_finished, now))
			return (NULL);
		if (data->compiles_required != -1 && all_finished)
		{
			data->simulation_end = 1;
			pthread_mutex_unlock(&data->state_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&data->state_mutex);
		usleep(1000);
	}
	return (NULL);
}
