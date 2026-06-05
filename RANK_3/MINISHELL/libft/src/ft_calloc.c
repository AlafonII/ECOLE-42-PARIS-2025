/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 08:55:05 by jguillem          #+#    #+#             */
/*   Updated: 2024/11/22 14:13:24 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stddef.h>
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*array;
	size_t	i;
	size_t	total;

	total = nmemb * size;
	if (!size || total / size != nmemb)
		return (NULL);
	i = 0;
	array = malloc(total);
	if (!array)
		return (NULL);
	while (array && i < size)
		*(unsigned char *)(array + i++) = 0;
	ft_bzero(array, nmemb);
	return (array);
}
