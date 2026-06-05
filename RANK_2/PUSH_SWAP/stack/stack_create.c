/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 14:38:32 by alafon            #+#    #+#             */
/*   Updated: 2025/03/01 12:56:44 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	create_stack_multiple_args(int argc, char **argv, t_stack *a)
{
	int	i;
	int	size;

	i = 0;
	size = argc - 1;
	a->arr = malloc(sizeof(int) * size);
	a->size = size;
	while (i < size)
	{
		a->arr[i] = ft_atoi(argv[i + 1]);
		i++;
	}
}

void	create_stack_single_arg(char *arg, t_stack *a)
{
	int		i;
	char	**stack;
	int		size;

	i = 0;
	stack = ft_split(arg, ' ');
	while (stack[i])
		i++;
	size = i;
	a->arr = malloc(sizeof(int) * size);
	a->size = size;
	i = 0;
	while (i < size)
	{
		a->arr[i] = ft_atoi(stack[i]);
		i++;
	}
	free_stack(stack);
}

void	stack_create(int argc, char **argv, t_stack *a)
{
	if (argc > 2)
		create_stack_multiple_args(argc, argv, a);
	else if (argc == 2)
		create_stack_single_arg(argv[1], a);
}
