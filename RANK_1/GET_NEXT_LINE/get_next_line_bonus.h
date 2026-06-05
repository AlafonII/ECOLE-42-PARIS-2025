/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:31:07 by alafon            #+#    #+#             */
/*   Updated: 2025/01/14 09:31:37 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 512
# endif
# if BUFFER_SIZE > 125000 || BUFFER_SIZE < 1
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 512
# endif

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

char	*gnl_strjoin(char *s1, char *s2, int *newline_position);
char	*get_next_line(int fd);
char	*read_newline(char *line, char *buffer, int *nl_pos, int fd);
char	*initialize_line(char *buffer, int *newline_position);

//-------------------------------------------------------//

void	gnl_strlcpy(char *dst, const char *src, size_t dst_size);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	ft_bzero(void *s, size_t n);

//-------------------------------------------------------//

size_t	ft_strlen(const char *str);
size_t	find_newline_position(char *buffer);

#endif