/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 08:30:25 by jguillem          #+#    #+#             */
/*   Updated: 2024/11/17 10:19:23 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stddef.h>

static int	num_len(long int nb)
{
	int			len;
	long int	pow;

	len = 1;
	pow = 10;
	while (nb && pow / nb <= 1)
	{
		len++;
		pow *= 10;
	}
	return (len);
}

static void	itoa_fill(char *itoa, long int nb, int len, int neg)
{
	itoa[len + neg] = '\0';
	if (nb == 0)
		itoa[--len + neg] = '0';
	while (nb > 0)
	{
		itoa[--len + neg] = nb % 10 + '0';
		nb /= 10;
	}
	if (neg)
		itoa[len] = '-';
}

char	*ft_itoa(int n)
{
	int			len;
	long int	nb;
	int			neg;
	char		*itoa;

	nb = (long int)n;
	if (nb < 0)
	{
		neg = 1;
		nb *= -1;
	}
	else
		neg = 0;
	len = num_len(nb);
	itoa = malloc(sizeof(char) * (len + 1 + neg));
	if (!itoa)
		return (NULL);
	itoa_fill(itoa, nb, len, neg);
	return (itoa);
}
