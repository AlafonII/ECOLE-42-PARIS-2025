/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:51:58 by alex              #+#    #+#             */
/*   Updated: 2025/06/12 20:20:14 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * ---------------------------------------------------------------
 * Filename: parse_data.c
 * ---------------------------------------------------------------
 * Trim spaces, process header block and parse .cub configuration.
 * ---------------------------------------------------------------
 */

/*
 * Function: trim_spaces
 * Role: Remove leading and trailing whitespace in place.
 *
 * Parameters:
 *   s - String to trim.
 *
 * Returns:
 *   Pointer to first non-space character.
 */
char	*trim_spaces(char *s)
{
	char	*end;

	while (ft_isspace((char)*s))
		s++;
	if (*s == '\0')
		return (s);
	end = s + ft_strlen(s) - 1;
	while (end > s && ft_isspace((char)*end))
	{
		*end = '\0';
		end--;
	}
	return (s);
}

/*
 * Function: process_header_line
 * Role: Parse one header line for textures or colors.
 *
 * Parameters:
 *   trim     - Trimmed header line.
 *   textures - Textures struct to populate.
 *
 * Returns:
 *   SUCCESS if parsed; otherwise FAILURE.
 */
static int	process_header_line(char *trim, t_textures *textures)
{
	if (*trim == '\0')
		return (FAILURE);
	if (ft_strncmp(trim, "NO", 2) == 0
		|| ft_strncmp(trim, "SO", 2) == 0
		|| ft_strncmp(trim, "WE", 2) == 0
		|| ft_strncmp(trim, "EA", 2) == 0)
	{
		if (parse_texture_headers(trim, textures) == FAILURE)
			return (FAILURE);
	}
	else if (trim[0] == 'F' || trim[0] == 'C')
	{
		if (parse_color_headers(trim, textures) == FAILURE)
			return (FAILURE);
	}
	else
		return (FAILURE);
	return (SUCCESS);
}

/*
 * Function: process_header_block
 * Role: Read six non-empty header lines and fetch next line.
 *
 * Parameters:
 *   fd        - File descriptor of .cub file.
 *   textures  - Textures struct to populate.
 *   nxt_line  - Output pointer for next unread line.
 *
 * Returns:
 *   SUCCESS if six headers parsed; otherwise FAILURE.
 */
static int	process_header_block(int fd, t_textures *textures, char **nxt_line)
{
	char	*line;
	int		count;

	count = 0;
	while (count < 6)
	{
		line = get_next_line(fd);
		while (line && *trim_spaces(line) == '\0')
		{
			free(line);
			line = get_next_line(fd);
		}
		if (!line
			|| process_header_line(trim_spaces(line), textures) == FAILURE)
		{
			if (line)
				free(line);
			return (FAILURE);
		}
		count++;
		free(line);
	}
	*nxt_line = get_next_line(fd);
	return (SUCCESS);
}

/*
 * Function: parse_headers
 * Role: Validate header block and ensure no extra header lines.
 *
 * Parameters:
 *   fd        - File descriptor of .cub file.
 *   textures  - Textures struct to populate.
 *
 * Returns:
 *   SUCCESS if headers valid; otherwise FAILURE.
 */
static int	parse_headers(int fd, t_textures *textures)
{
	char	*line;
	char	*trim;

	if (process_header_block(fd, textures, &line) == FAILURE)
		return (FAILURE);
	if (line)
	{
		trim = trim_spaces(line);
		if (*trim != '\0'
			&& (ft_strncmp(trim, "NO", 2) == 0
				|| ft_strncmp(trim, "SO", 2) == 0
				|| ft_strncmp(trim, "WE", 2) == 0
				|| ft_strncmp(trim, "EA", 2) == 0
				|| trim[0] == 'F'
				|| trim[0] == 'C'))
		{
			free(line);
			return (FAILURE);
		}
		free(line);
	}
	return (SUCCESS);
}

/*
 * Function: parse_cub_file
 * Role: Open .cub file, parse headers, textures, and map into data.
 *
 * Parameters:
 *   path - Path to .cub configuration file.
 *
 * Returns:
 *   Pointer to t_data with parsed map and textures; exits on error.
 */
t_data	*parse_cub_file(const char *path)
{
	int			fd;
	t_data		*cfg;
	t_mapinfo	info;
	char		**map_lines;

	if (map_file_check(path) == FAILURE)
		error_exit(FILE_OPEN_ERROR);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		error_exit(FILE_OPEN_ERROR);
	cfg = init_data();
	if (parse_headers(fd, &cfg->textures) == FAILURE)
		cleanup_and_exit(cfg, fd, DEF_ERROR);
	if (validate_all_textures(&cfg->textures) == FAILURE)
		cleanup_and_exit(cfg, fd, TXTR_ERROR);
	mapinfo_init(&info, fd, path);
	if (parse_map_lines(&info) == FAILURE)
		cleanup_and_exit(cfg, fd, MAP_ROW_ERROR);
	map_lines = info.file;
	if (validate_map(map_lines, cfg) == FAILURE)
	{
		free_map(map_lines);
		cleanup_and_exit(cfg, fd, MAP_ERROR);
	}
	return (close(fd), cfg->map = map_lines, cfg);
}
