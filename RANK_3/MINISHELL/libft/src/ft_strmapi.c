/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 12:22:57 by jguillem          #+#    #+#             */
/*   Updated: 2024/11/20 17:26:24 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stddef.h>
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	len;
	char	*strmapi;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	strmapi = malloc(sizeof(char) * (len + 1));
	if (!strmapi)
		return (NULL);
	strmapi[len] = '\0';
	while (len--)
		strmapi[len] = (*f)(len, s[len]);
	return (strmapi);
}
