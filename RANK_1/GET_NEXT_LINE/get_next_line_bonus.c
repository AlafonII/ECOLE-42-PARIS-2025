/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:11:22 by alafon            #+#    #+#             */
/*   Updated: 2025/01/14 09:49:40 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*initialize_line(char *buffer, int *newline_position)
{
	size_t	len;
	char	*line;

	len = 0;
	while (buffer[len] && buffer[len] != '\n')
		len++;
	len++;
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	ft_memcpy(line, buffer, len);
	line[len] = '\0';
	if (len > 0 && line[len - 1] == '\n')
		*newline_position = len - 1;
	return (line);
}

size_t	find_newline_position(char *buffer)
{
	size_t	index;

	index = 0;
	if (!buffer)
		return (-1);
	while (index < BUFFER_SIZE)
	{
		if (buffer[index] == '\n' || buffer[index] == '\0')
			return (index + 1);
		index++;
	}
	return (index);
}

char	*read_newline(char *line, char *buffer, int *nl_pos, int fd)
{
	char	tmp_buffer[BUFFER_SIZE + 1];
	ssize_t	bytes_read;
	size_t	newline_index;

	while (*nl_pos == -1)
	{
		ft_bzero(tmp_buffer, (BUFFER_SIZE + 1));
		bytes_read = read(fd, tmp_buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(line), ft_bzero(buffer, (BUFFER_SIZE + 1)), NULL);
		newline_index = find_newline_position(tmp_buffer);
		gnl_strlcpy(buffer, &tmp_buffer[newline_index], (BUFFER_SIZE + 1));
		tmp_buffer[newline_index] = '\0';
		line = gnl_strjoin(line, tmp_buffer, nl_pos);
		if (bytes_read == 0)
		{
			ft_bzero(buffer, BUFFER_SIZE + 1);
			break ;
		}
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	buffer[1024][BUFFER_SIZE + 1];
	char		*line;
	int			newline_position;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	newline_position = -1;
	line = initialize_line(buffer[fd], &newline_position);
	if (!line)
		return (NULL);
	gnl_strlcpy(buffer[fd], &buffer[fd][newline_position + 1], BUFFER_SIZE + 1);
	line = read_newline(line, buffer[fd], &newline_position, fd);
	if (!line || line[0] == '\0')
		return (free(line), NULL);
	return (line);
}

// int	main(void)
// {
// 	int		fd = open("get_next_line.h", O_RDONLY);
// 	char	*line;
// 	int		i;

// 	i = 0;
// 	while (i < 10)
// 	{
// 		line = get_next_line(fd);
// 		printf("%s %d", line, BUFFER_SIZE);
// 		free(line);
// 		i++;
// 	}
// 	close(fd);
// 	return (0);
// }