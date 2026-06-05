/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:52:12 by jguillem          #+#    #+#             */
/*   Updated: 2024/11/22 14:45:51 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"

static size_t	min(size_t size, size_t dlen)
{
	if (size < dlen)
		return (size);
	return (dlen);
}

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	dlen;
	size_t	cursor;

	dlen = 0;
	while (dest[dlen] && dlen < size)
		dlen++;
	if (size == 0 || dlen >= size - 1)
		return (ft_strlen(src) + min(size, dlen));
	cursor = 0;
	while (src[cursor] && cursor + dlen < size - 1)
	{
		dest[dlen + cursor] = src[cursor];
		cursor++;
	}
	dest[dlen + cursor] = '\0';
	return (ft_strlen(src) + min(size, dlen));
}
