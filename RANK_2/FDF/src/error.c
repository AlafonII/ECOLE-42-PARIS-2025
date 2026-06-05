/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 17:48:25 by alex              #+#    #+#             */
/*   Updated: 2025/02/24 09:15:01 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

static int	is_valid_value_trimmed(const char *token)
{
	char	*trimmed;
	int		valid;

	trimmed = ft_strtrim(token, " \t\n\r\v\f");
	if (!trimmed)
		return (0);
	valid = (trimmed[0] != '\0');
	free(trimmed);
	return (valid);
}

int	check_line_count(char **tokens, int expected)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (tokens[i])
	{
		if (is_valid_value_trimmed(tokens[i]))
			count++;
		i++;
	}
	if (count != expected)
	{
		ft_putendl_fd("Error", 2);
		return (0);
	}
	return (1);
}
