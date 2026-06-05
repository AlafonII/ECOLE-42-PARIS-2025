/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 10:42:50 by alafon            #+#    #+#             */
/*   Updated: 2025/03/01 12:39:50 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

int	stack_size(int argc, char **argv)
{
	int		i;
	char	**stack;

	i = 0;
	if (argc > 2)
		return (argc - 1);
	else if (argc == 2)
	{
		stack = ft_split(argv[1], ' ');
		while (stack[i])
			i++;
		free_stack(stack);
	}
	return (i);
}
