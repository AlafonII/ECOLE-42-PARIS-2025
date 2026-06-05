/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 12:46:11 by alex              #+#    #+#             */
/*   Updated: 2025/03/01 13:06:06 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

int	find_min_index(t_stack *a)
{
	int	i;
	int	min_idx;

	i = 0;
	min_idx = 0;
	while (i < a->size)
	{
		if (a->arr[i] < a->arr[min_idx])
			min_idx = i;
		i++;
	}
	return (min_idx);
}

void	push_min_to_b(t_stack *a, t_stack *b)
{
	int	min_idx;

	min_idx = find_min_index(a);
	while (min_idx != 0)
	{
		if (min_idx <= a->size / 2)
			ra(a);
		else
			rra(a);
		min_idx = find_min_index(a);
	}
	pb(a, b);
}

void	sort_three(t_stack *a)
{
	if (a->size < 2)
		return ;
	if (a->arr[0] > a->arr[1] && a->arr[0] > a->arr[2])
		ra(a);
	else if (a->arr[1] > a->arr[0] && a->arr[1] > a->arr[2])
		rra(a);
	if (a->arr[0] > a->arr[1])
		sa(a);
}

int	is_sorted(t_stack *a)
{
	int	i;

	i = 0;
	while (i < a->size - 1)
	{
		if (a->arr[i] > a->arr[i + 1])
			return (0);
		i++;
	}
	return (1);
}
