/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   turk_sort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 16:37:34 by alex              #+#    #+#             */
/*   Updated: 2025/02/25 16:38:06 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	push_chunk_to_b(t_stack *a, t_stack *b, t_chunk c)
{
	int	pushed;
	int	inspected;
	int	init_size;

	pushed = 0;
	inspected = 0;
	init_size = a->size;
	while (inspected < init_size && pushed < c.need)
	{
		if (a->arr[0] <= c.pivot)
		{
			pb(a, b);
			pushed++;
		}
		else
			ra(a);
		inspected++;
	}
}

int	find_max_index(t_stack *b)
{
	int	max;
	int	idx;
	int	i;

	max = b->arr[0];
	idx = 0;
	i = 1;
	while (i < b->size)
	{
		if (b->arr[i] > max)
		{
			max = b->arr[i];
			idx = i;
		}
		i++;
	}
	return (idx);
}

void	push_all_back_to_a(t_stack *a, t_stack *b)
{
	int	idx;

	while (b->size > 0)
	{
		idx = find_max_index(b);
		if (idx <= b->size / 2)
			while (idx-- > 0)
				rb(b);
		else
			while (idx++ < b->size)
				rrb(b);
		pa(a, b);
	}
}

void	process_chunks(t_data *d, t_stack *a, t_stack *b)
{
	int		i;
	t_chunk	c;
	int		idx;

	i = 0;
	while (i < d->chunk_cnt && a->size > 0)
	{
		idx = (i + 1) * d->chunk_sz - 1;
		if (idx >= d->n)
			idx = d->n - 1;
		c.pivot = d->sorted[idx];
		c.need = chunk_need(i, *d);
		push_chunk_to_b(a, b, c);
		d->total_pushed += c.need;
		i++;
	}
}

void	turk_sort(t_stack *a, t_stack *b)
{
	t_data	d;
	int		*sorted;

	d.n = a->size;
	if (d.n < 2)
		return ;
	sorted = create_sorted_copy(a);
	if (!sorted)
		return ;
	d.sorted = sorted;
	d.chunk_cnt = compute_chunk_count(d.n);
	d.chunk_sz = compute_chunk_size(d.n, d.chunk_cnt);
	d.total_pushed = 0;
	process_chunks(&d, a, b);
	push_all_back_to_a(a, b);
	free(sorted);
}
