/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguillem <jguillem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 19:29:15 by jguillem          #+#    #+#             */
/*   Updated: 2024/11/20 17:09:25 by jguillem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stddef.h>
#include "libft.h"

static int	find_nb_words(char const *s, char sep)
{
	int	i;
	int	nb;

	i = 0;
	nb = 0;
	while (s[i])
	{
		while (s[i] && s[i] == sep)
			i++;
		if (s[i])
			nb++;
		while (s[i] && s[i] != sep)
			i++;
	}
	return (nb);
}

char	**ft_split(char const *s, char c)
{
	int		start;
	int		end;
	int		nb_words;
	char	**split;

	nb_words = find_nb_words(s, c);
	split = malloc(sizeof(char *) * (nb_words + 1));
	if (!split || !s)
		return (NULL);
	split[nb_words] = NULL;
	end = ft_strlen(s) - 1;
	while (nb_words-- > 0)
	{
		while (end >= 0 && s[end] == c)
			end--;
		start = end;
		while (start >= 0 && s[start] != c)
			start--;
		split[nb_words] = ft_substr(s, start + 1, end - start);
		end = start;
	}
	return (split);
}
