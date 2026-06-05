/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 18:09:46 by jguillem          #+#    #+#             */
/*   Updated: 2024/11/20 16:34:22 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include <stddef.h>

static int	ft_isin(char *s1, char c)
{
	while (*s1)
		if (c == *s1++)
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*strtrim;

	if (!s1)
		return (NULL);
	start = 0;
	end = ft_strlen(s1) - 1;
	if (set)
	{
		while (s1[start] && ft_isin((char *)set, s1[start]))
			start++;
		while (ft_isin((char *)set, s1[end]) && end > start)
			end--;
	}
	strtrim = malloc(sizeof(char) * (end - start + 2));
	if (!strtrim)
		return (NULL);
	strtrim[end - start + 1] = '\0';
	while (start <= end)
	{
		*(strtrim + (end - start)) = *(s1 + end);
		end--;
	}
	return (strtrim);
}
