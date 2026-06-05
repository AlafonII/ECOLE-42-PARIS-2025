/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:40:57 by jguillem          #+#    #+#             */
/*   Updated: 2024/11/20 16:09:26 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	char	*ret;
	size_t	sub_len;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		sub_len = 0;
	else
		sub_len = ft_strlen(s) - start;
	if (sub_len > len)
		sub_len = len;
	substr = malloc(sizeof(char) * (sub_len + 1));
	if (!substr)
		return (NULL);
	ret = substr;
	while (sub_len-- && *(s + start))
		*(substr++) = *(s + start++);
	*substr = '\0';
	return (ret);
}
