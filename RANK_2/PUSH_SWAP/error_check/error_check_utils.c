/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_check_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:34:51 by alafon            #+#    #+#             */
/*   Updated: 2025/03/01 12:41:21 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

int	ft_is_int(const char *str)
{
	int	i;

	if (!str[0])
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_in_int_range(const char *str)
{
	long long	num;
	int			sign;
	int			i;

	num = 0;
	sign = 1;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		num = num * 10 + (str[i] - '0');
		if ((sign == 1 && num > 2147483647LL)
			|| (sign == -1 && num > 2147483648LL))
			return (0);
		i++;
	}
	return (1);
}

int	ft_has_duplicate(int *arr, int size)
{
	int	i;
	int	j;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (arr[i] == arr[j])
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	count_args(int argc, char **argv)
{
	int		i;
	int		count;
	int		j;
	char	**split;

	count = 0;
	i = 1;
	while (i < argc)
	{
		split = ft_split(argv[i++], ' ');
		j = 0;
		while (split[j])
			j++;
		count += j;
		free_stack(split);
	}
	return (count);
}

int	fill_array(int argc, char **argv, int *arr)
{
	int		i;
	int		j;
	int		idx;
	char	**split;

	i = 1;
	idx = 0;
	while (i < argc)
	{
		split = ft_split(argv[i++], ' ');
		j = 0;
		while (split[j])
		{
			if (!ft_is_int(split[j]) || !ft_in_int_range(split[j]))
				return (free_stack(split), 0);
			arr[idx++] = ft_atoi(split[j++]);
		}
		free_stack(split);
	}
	return (1);
}
