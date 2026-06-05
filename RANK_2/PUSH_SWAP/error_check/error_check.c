/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:55:08 by alafon            #+#    #+#             */
/*   Updated: 2025/03/01 12:40:51 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

int	ft_check_double_strings(int argc, char **argv)
{
	int		i;
	int		j;
	int		multi_count;
	char	**split;

	i = 1;
	multi_count = 0;
	while (i < argc)
	{
		split = ft_split(argv[i], ' ');
		j = 0;
		while (split[j])
			j++;
		if (j > 1)
			multi_count++;
		free_stack(split);
		if (multi_count > 1)
			return (0);
		i++;
	}
	return (1);
}

int	ft_error_checker(int argc, char **argv)
{
	int	total;
	int	*arr;

	if (argc < 2)
		return (0);
	if (!ft_check_double_strings(argc, argv))
		return (0);
	total = count_args(argc, argv);
	arr = malloc(sizeof(int) * total);
	if (!arr)
		return (0);
	if (!fill_array(argc, argv, arr))
		return (free(arr), 0);
	if (ft_has_duplicate(arr, total))
		return (free(arr), 0);
	free(arr);
	return (1);
}
