/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:22:53 by jguillem          #+#    #+#             */
/*   Updated: 2024/11/20 16:19:20 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	l1;
	size_t	l2;
	char	*strjoin;

	if (!s1 && !s2)
		return (NULL);
	if (s1)
		l1 = ft_strlen(s1);
	else
		l1 = 0;
	if (s2)
		l2 = ft_strlen(s2);
	else
		l2 = 0;
	strjoin = malloc(sizeof(char) * (l1 + l2 + 1));
	if (!strjoin)
		return (NULL);
	*(strjoin + l1 + l2) = '\0';
	while (l2-- > 0)
		*(strjoin + l1 + l2) = *(s2 + l2);
	while (l1-- > 0)
		*(strjoin + l1) = *(s1 + l1);
	return (strjoin);
}
