/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:31:26 by alafon            #+#    #+#             */
/*   Updated: 2024/11/18 13:43:02 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *ptr)
{
	int	i;
	int	is_neg;
	int	res;

	i = 0;
	is_neg = 1;
	while (ptr[i] == '\t' || ptr[i] == '\n' || ptr[i] == '\v'
		|| ptr[i] == '\f' || ptr[i] == '\r' || ptr[i] == ' ')
		i++;
	if (ptr[i] == '-' || ptr[i] == '+')
	{
		if (ptr[i] == '-')
			is_neg = -1;
		i++;
	}
	if (ptr[i] == '-' || ptr[i] == '+')
		return (0);
	res = 0;
	while (ptr[i] >= '0' && ptr[i] <= '9')
	{
		res = (res * 10) + (ptr[i] - '0');
		i++;
	}
	return (res * is_neg);
}
