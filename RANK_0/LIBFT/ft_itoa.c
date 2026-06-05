/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 21:57:42 by alex              #+#    #+#             */
/*   Updated: 2024/11/19 14:16:12 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	long	temp;
	int		len;
	char	*str;

	temp = n;
	len = get_len(temp);
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (temp < 0)
	{
		temp = -temp;
		str[0] = '-';
	}
	while (len > 0)
	{
		len--;
		if (str[len] == '-')
			break ;
		str[len] = (temp % 10) + '0';
		temp /= 10;
	}
	return (str);
}
