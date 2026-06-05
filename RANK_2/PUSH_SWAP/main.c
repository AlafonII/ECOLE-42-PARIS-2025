/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:24:25 by alafon            #+#    #+#             */
/*   Updated: 2025/03/01 13:05:16 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	main(int argc, char **argv)
{
	t_stack	a;
	t_stack	b;
	int		size;

	if (argc < 2)
		return (0);
	if (!ft_error_checker(argc, argv))
		return (write(2, "Error\n", 6), 0);
	size = stack_size(argc, argv);
	stack_create(argc, argv, &a);
	b.arr = malloc(sizeof(int) * size);
	b.size = 0;
	if (!a.arr || !b.arr)
		return (write(2, "Error\n", 6), free(a.arr), free(b.arr), 0);
	if (is_sorted(&a))
		return (free(a.arr), free(b.arr), 0);
	if (a.size <= 10)
		small_sort(&a, &b);
	else
		turk_sort(&a, &b);
	free(a.arr);
	free(b.arr);
	return (0);
}
