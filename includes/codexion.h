/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csekakul <csekakul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 09:02:12 by csekakul          #+#    #+#             */
/*   Updated: 2026/08/14 13:45:33 by csekakul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

struct	s_data;

/* ========================================================================== */
/*                                ENUMS & TYPES                               */
/* ========================================================================== */

typedef enum e_scheduler
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
}	t_scheduler;

typedef struct s_request
{
	int			coder_id;
	long long	key;
}	t_request;

typedef struct s_heap
{
	t_request	*data;
	int			capacity;
	int			size;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				is_in_use;
	long long		available_at;
	t_heap			queue;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	long long		last_compile_start;
	int				compiles_count;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	struct s_data	*data;
}	t_coder;

typedef struct s_data
{
	int				num_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	int				compiles_required;
	long long		dongle_cooldown;
	t_scheduler		scheduler;

	long long		start_time;
	int				simulation_end;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	log_mutex;

	t_dongle		*dongles;
	t_coder			*coders;
	pthread_t		monitor;
}	t_data;

/* ========================================================================== */
/*                             FUNCTION PROTOTYPES                            */
/* ========================================================================== */

/* --- Min-Heap Utilities (srcs/heap.c) --- */
int			heap_init(t_heap *heap, int capacity);
static int	is_higher_priority(t_request a, t_request b);
void		heap_push(t_heap *heap, int coder_id, long long key);
t_request	heap_peek(t_heap *heap);
void		heap_destroy(t_heap *heap);

long long	get_time_ms(void);
void		precise_sleep(long long duration_ms, t_data *data);
void		safe_print(t_data *data, int coder_id, const char *str);

#endif
