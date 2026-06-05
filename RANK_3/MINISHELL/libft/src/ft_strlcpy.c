/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:11:42 by jguillem          #+#    #+#             */
/*   Updated: 2024/11/13 15:51:01 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dsize)
{
	size_t	ret;

	ret = 0;
	if (!dsize)
		return (ft_strlen(src));
	while (src[ret] && ret < dsize - 1)
	{
		dst[ret] = src[ret];
		ret++;
	}
	dst[ret] = '\0';
	return (ft_strlen(src));
}
