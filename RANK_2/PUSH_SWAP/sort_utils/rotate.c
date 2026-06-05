/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:21:05 by alafon            #+#    #+#             */
/*   Updated: 2025/02/25 15:15:49 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static void	rotate_stack(t_stack *stack)
{
	int	first;
	int	i;

	if (stack->size > 1)
	{
		first = stack->arr[0];
		i = 0;
		while (i < stack->size - 1)
		{
			stack->arr[i] = stack->arr[i + 1];
			i++;
		}
		stack->arr[stack->size - 1] = first;
	}
}

void	ra(t_stack *a)
{
	rotate_stack(a);
	ft_printf("ra\n");
}

void	rb(t_stack *b)
{
	rotate_stack(b);
	ft_printf("rb\n");
}

void	rr(t_stack *a, t_stack *b)
{
	rotate_stack(a);
	rotate_stack(b);
	ft_printf("rr\n");
}
