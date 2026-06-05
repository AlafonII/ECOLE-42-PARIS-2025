/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   turk_sort_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 10:19:48 by alafon            #+#    #+#             */
/*   Updated: 2025/03/04 09:41:01 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	sort_array(int *arr, int n)
{
	int	i;
	int	j;
	int	tmp;

	i = 0;
	while (i < n - 1)
	{
		j = i + 1;
		while (j < n)
		{
			if (arr[i] > arr[j])
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	*create_sorted_copy(t_stack *a)
{
	int	*sorted;
	int	i;

	sorted = malloc(sizeof(int) * a->size);
	if (!sorted)
		return (NULL);
	i = 0;
	while (i < a->size)
	{
		sorted[i] = a->arr[i];
		i++;
	}
	sort_array(sorted, a->size);
	return (sorted);
}

int	compute_chunk_count(int n)
{
	if (n <= 15)
		return (3);
	else if (n <= 100)
		return (5);
	else if (n <= 500)
		return (15);
	else
		return (n / 35);
}

int	compute_chunk_size(int n, int chunk_count)
{
	return ((n + chunk_count - 1) / chunk_count);
}

int	chunk_need(int i, t_data d)
{
	int	limit;

	if (i == d.chunk_cnt - 1)
		limit = d.n;
	else
		limit = (i + 1) * d.chunk_sz;
	if (limit > d.n)
		limit = d.n;
	return (limit - d.total_pushed);
}
