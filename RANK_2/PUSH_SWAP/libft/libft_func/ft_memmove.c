/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 21:06:16 by alex              #+#    #+#             */
/*   Updated: 2024/11/19 13:50:46 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	const unsigned char	*tmp_src;
	unsigned char		*tmp_dst;

	if (!dst && !src)
		return (NULL);
	tmp_src = (const unsigned char *)src;
	tmp_dst = (unsigned char *)dst;
	if (tmp_dst < tmp_src)
		while (len--)
			*tmp_dst++ = *tmp_src++;
	else
		while (len--)
			tmp_dst[len] = tmp_src[len];
	return (dst);
}
