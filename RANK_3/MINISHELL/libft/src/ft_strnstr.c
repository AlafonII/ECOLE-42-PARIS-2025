/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:39:12 by jguillem          #+#    #+#             */
/*   Updated: 2024/11/20 20:01:02 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	llen;

	i = 0;
	llen = ft_strlen(little);
	if (!llen)
		return ((char *)big);
	while (i + llen < len && big[i] && ft_strncmp(little, big + i, llen))
		i++;
	if (!ft_strncmp(little, big + i, llen) && len > 0)
		return ((char *)big + i);
	return (0);
}
