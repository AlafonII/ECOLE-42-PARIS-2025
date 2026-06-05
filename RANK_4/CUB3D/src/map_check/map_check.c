/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:05:42 by alex              #+#    #+#             */
/*   Updated: 2025/06/12 16:10:54 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	**dup_str_array(char **arr)
{
	int		count;
	int		i;
	char	**dup;

	count = 0;
	while (arr[count])
		count++;
	dup = ft_calloc(count + 1, sizeof(char *));
	if (!dup)
		error_exit(MEMORY_ERROR);
	i = 0;
	while (i < count)
	{
		dup[i] = ft_strdup(arr[i]);
		if (!dup[i])
			error_exit(MEMORY_ERROR);
		i++;
	}
	dup[count] = NULL;
	return (dup);
}

int	is_map_row(char *line)
{
	int	i;

	i = 0;
	while (ft_isspace((char)line[i]))
		i++;
	if (line[i] == '\0')
		return (0);
	while (line[i])
	{
		if (!(line[i] == '0' || line[i] == '1'
				|| line[i] == 'N' || line[i] == 'S'
				|| line[i] == 'E' || line[i] == 'W'
				|| line[i] == ' '))
			return (0);
		i++;
	}
	return (1);
}

int	validate_map(char **map_lines, t_data *data)
{
	char	**copy;

	copy = dup_str_array(map_lines);
	copy = replace_whitespace(copy);
	if (check_no_leak(copy) == FAILURE)
	{
		free_map(copy);
		return (FAILURE);
	}
	player_init(&data->player);
	if (check_and_set_player(copy, data) == FAILURE)
	{
		free_map(copy);
		return (FAILURE);
	}
	free_map(copy);
	return (SUCCESS);
}
