/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csekakul <csekakul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 09:06:15 by csekakul          #+#    #+#             */
/*   Updated: 2026/08/14 11:37:58 by csekakul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void	precise_sleep(long long duration_ms, t_data *data)
{
	long long	start;

	start = get_time_ms();
	while (get_time_ms() - start < duration_ms)
	{
		if (is_simulation_over(data))
			break ;
		usleep(1000);
	}
}

void	safe_print(t_data *data, int coder_id, const char *str)
{
	long long	now;

	pthread_mutex_lock(&data->log_mutex);
	pthread_mutex_lock(&data->state_mutex);
	if (!data->simulation_end)
	{
		now = get_time_ms() - data->start_time;
		printf("%lld %d %s\n", now, coder_id, str);
	}
	pthread_mutex_unlock(&data->state_mutex);
	pthread_mutex_unlock(&data->log_mutex);
}
