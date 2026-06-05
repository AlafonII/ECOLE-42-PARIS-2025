/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:36:18 by alex              #+#    #+#             */
/*   Updated: 2025/02/23 17:24:14 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

int	is_line_empty(const char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n'
			&& line[i] != '\v' && line[i] != '\f' && line[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}

int	count_columns(const char *line)
{
	int		count;
	char	**split;
	char	*trimmed_line;

	trimmed_line = ft_strtrim(line, " \t\n\r\v\f");
	if (!trimmed_line)
		return (0);
	split = ft_split(trimmed_line, ' ');
	free(trimmed_line);
	if (!split)
		return (0);
	count = 0;
	while (split[count])
		count++;
	free_split(split);
	return (count);
}

t_point3d	*parse_line(char *line, int y, int cols)
{
	t_point3d	*row;
	char		**split;
	int			x;

	split = ft_split(line, ' ');
	if (!split)
		return (free(line), NULL);
	if (!check_line_count(split, cols))
		return (free_split(split), free(line), NULL);
	row = malloc(sizeof(t_point3d) * cols);
	if (!row)
		return (free_split(split), free(line), NULL);
	x = 0;
	while (x < cols)
	{
		row[x].x = x;
		row[x].y = y;
		row[x].z = ft_atoi(split[x]);
		x++;
	}
	return (free_split(split), free(line), row);
}
