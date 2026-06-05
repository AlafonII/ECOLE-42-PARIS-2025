/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 19:49:07 by alex              #+#    #+#             */
/*   Updated: 2024/12/03 20:17:41 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_putstr(char *str)
{
	int	len;

	len = 0;
	if (!str)
	{
		write(1, "(null)", 6);
		return (6);
	}
	while (*str != '\0')
	{
		write(1, str, 1);
		str++;
		len++;
	}
	return (len);
}

int	ft_putnbr(long nbr)
{
	int	len;

	len = 0;
	if (nbr < 0)
	{
		write(1, "-", 1);
		len++;
		nbr = -nbr;
	}
	if (nbr >= 10)
	{
		len += ft_putnbr(nbr / 10);
		len += ft_putchar(nbr % 10 + '0');
	}
	else
		len += ft_putchar(nbr + '0');
	return (len);
}

int	ft_putnbr_base(unsigned long nbr, char *base)
{
	int				len;
	unsigned long	base_len;

	len = 0;
	base_len = 0;
	while (base[base_len] != '\0')
		base_len++;
	if (nbr >= base_len)
	{
		len += ft_putnbr_base(nbr / base_len, base);
		len += ft_putchar(base[nbr % base_len]);
	}
	else
		len += ft_putchar(base[nbr]);
	return (len);
}

int	ft_putpointer(unsigned long ptr, char *base)
{
	int	len;

	len = 0;
	if (ptr == 0)
		return (ft_putstr("(nil)"));
	len += ft_putstr("0x");
	len += ft_putnbr_base(ptr, base);
	return (len);
}
