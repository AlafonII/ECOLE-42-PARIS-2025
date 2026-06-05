/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:54:42 by alafon            #+#    #+#             */
/*   Updated: 2024/11/12 15:15:17 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t n)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;
	size_t	len;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	len = dst_len;
	i = 0;
	if (dst_len < n - 1 && n > 0)
	{
		while (src[i] && dst_len + i < n - 1)
		{
			dst[len] = src[i];
			len++;
			i++;
		}
		dst[len] = 0;
	}
	if (dst_len >= n)
		dst_len = n;
	return (dst_len + src_len);
}
