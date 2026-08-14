/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csekakul <csekakul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 09:06:04 by csekakul          #+#    #+#             */
/*   Updated: 2026/08/14 13:08:37 by csekakul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *heap, int capacity)
{
	heap->data = malloc(sizeof(t_request) * capacity);
	if (!heap->data)
		return (0);
	heap->capacity = capacity;
	heap->size = 0;
	return (1);
}

static int	is_higher_priority(t_request a, t_request b)
{
	if (a.key < b.key)
		return (1);
	if (a.key > b.key)
		return (0);
	return (a.coder_id < b.coder_id);
}

void	heap_push(t_heap *heap, int coder_id, long long key)
{
	int			i;
	int			parent;
	t_request	tmp;

	if (heap->size >= heap->capacity)
		return ;
	i = heap->size;
	heap->data[i].coder_id = coder_id;
	heap->data[i].key = key;
	heap->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (is_higher_priority(heap->data[i], heap->data[parent]))
		{
			tmp = heap->data[i];
			heap->data[i] = heap->data[parent];
			heap->data[parent] = tmp;
			i = parent;
		}
		else
			break ;
	}
}

t_request	heap_peek(t_heap *heap)
{
	t_request	empty;

	empty.coder_id = -1;
	empty.key = -1;
	if (heap->size <= 0)
		return (empty);
	return (heap->data[0]);
}

void	heap_destroy(t_heap *heap)
{
	if (heap->data)
	{
		free(heap->data);
		heap->data = NULL;
	}
	heap->size = 0;
}
