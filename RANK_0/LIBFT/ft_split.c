/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:20:28 by alafon            #+#    #+#             */
/*   Updated: 2024/11/19 18:45:18 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		word_len(char const *s, char c);
static int		word_count(char const *s, char c);
static char		**ft_tab(char const *s, char c);
static char		**mini_split(char const *s, char c, char **tab);

char	**ft_split(char const *s, char c)
{
	char	**tab;

	tab = ft_tab(s, c);
	if (!tab)
		return (NULL);
	tab = mini_split(s, c, tab);
	return (tab);
}

static char	**mini_split(char const *s, char c, char **tab)
{
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i] != 0)
		{
			tab[j] = malloc((word_len(&s[i], c) + 1) * sizeof(char));
			if (!tab[j])
				return (NULL);
			k = 0;
			while (s[i] != c && s[i] != 0)
				tab[j][k++] = s[i++];
			tab[j++][k] = 0;
		}
	}
	tab[j] = 0;
	return (tab);
}

static char	**ft_tab(char const *s, char c)
{
	char	**tab;

	if (!s)
		return (NULL);
	tab = malloc((word_count(s, c) + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	return (tab);
}

static int	word_count(char const *s, char c)
{
	int		i;
	int		count;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i] != 0)
			count++;
		while (s[i] != c && s[i] != 0)
			i++;
	}
	return (count);
}

static int	word_len(char const *s, char c)
{
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (s[i] == c)
		i++;
	while (s[i] != c && s[i] != 0)
	{
		count++;
		i++;
	}
	return (count);
}
