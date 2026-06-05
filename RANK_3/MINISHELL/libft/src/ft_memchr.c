/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 19:24:25 by jguillem          #+#    #+#             */
/*   Updated: 2024/11/20 18:33:20 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	if (!s || !n)
		return (NULL);
	i = 0;
	while (i < n && (unsigned char)c != *((unsigned char *)s + i))
		i++;
	if (*((unsigned char *)s + i) == (unsigned char)c)
		return ((void *)(s + i));
	return (0);
}
