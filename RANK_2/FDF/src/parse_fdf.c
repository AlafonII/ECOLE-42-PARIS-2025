/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fdf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:22:06 by alafon            #+#    #+#             */
/*   Updated: 2025/02/24 09:48:13 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

static int	read_first_valid_line(int fd, t_point3d **map,
	int *rows, int *cols)
{
	char	*line;

	line = get_next_line(fd);
	while (line && is_line_empty(line))
	{
		free(line);
		line = get_next_line(fd);
	}
	if (!line)
		return (0);
	*cols = count_columns(line);
	map[*rows] = parse_line(line, *rows, *cols);
	if (!map[*rows])
		return (0);
	(*rows)++;
	return (1);
}

static int	read_remaining_lines(int fd, t_point3d **map,
	int *rows, int cols)
{
	char	*line;
	int		current_row;

	current_row = *rows;
	line = get_next_line(fd);
	while (line)
	{
		if (is_line_empty(line))
		{
			free(line);
		}
		else
		{
			map[current_row] = parse_line(line, current_row, cols);
			if (!map[current_row])
				return (0);
			current_row++;
			*rows = current_row;
		}
		line = get_next_line(fd);
	}
	return (1);
}

t_point3d	**parse_fdf(char *filename, int *rows, int *cols)
{
	int			fd;
	t_point3d	**map;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	map = malloc(sizeof(t_point3d *) * 100000);
	if (!map)
		return (NULL);
	*rows = 0;
	if (!read_first_valid_line(fd, map, rows, cols))
		return (close(fd), free(map), NULL);
	if (!read_remaining_lines(fd, map, rows, *cols))
	{
		close(fd);
		while (*rows > 0)
		{
			(*rows)--;
			free(map[*rows]);
		}
		return (free(map), NULL);
	}
	return (close(fd), map);
}
