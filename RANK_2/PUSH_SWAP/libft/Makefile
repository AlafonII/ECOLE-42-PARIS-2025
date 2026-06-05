# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alafon <alafon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/08 16:55:51 by alex              #+#    #+#              #
#    Updated: 2024/12/17 12:02:01 by alafon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRC = libft_func/ft_strlen.c libft_func/ft_isalnum.c libft_func/ft_isalpha.c libft_func/ft_isascii.c libft_func/ft_isdigit.c\
libft_func/ft_isprint.c libft_func/ft_memset.c libft_func/ft_bzero.c libft_func/ft_memcpy.c libft_func/ft_memmove.c libft_func/ft_strlcpy.c\
libft_func/ft_toupper.c libft_func/ft_tolower.c libft_func/ft_strchr.c libft_func/ft_strrchr.c libft_func/ft_strncmp.c libft_func/ft_memchr.c\
libft_func/ft_memcmp.c libft_func/ft_strnstr.c libft_func/ft_atoi.c libft_func/ft_strlcat.c libft_func/ft_strlcpy.c libft_func/ft_calloc.c libft_func/ft_strdup.c\

SRC_ADD = libft_func/ft_substr.c libft_func/ft_strjoin.c libft_func/ft_strtrim.c libft_func/ft_split.c\
libft_func/ft_strmapi.c libft_func/ft_putchar_fd.c libft_func/ft_putstr_fd.c libft_func/ft_putendl_fd.c libft_func/ft_putnbr_fd.c\
libft_func/ft_striteri.c libft_func/ft_itoa.c

BONUS = libft_func/ft_lstnew.c libft_func/ft_lstadd_front.c libft_func/ft_lstsize.c libft_func/ft_lstlast.c libft_func/ft_lstadd_back.c\
libft_func/ft_lstdelone.c libft_func/ft_lstclear.c libft_func/ft_lstiter.c libft_func/ft_lstmap.c

PRINTF = ft_printf/ft_printf_utils.c ft_printf/ft_printf.c

OBJ = $(SRC:.c=.o)
OBJADD = $(SRC_ADD:.c=.o)
OBJBONUS = $(BONUS:.c=.o)
OBJPRINT = $(PRINTF:.c=.o)
OBJALL = $(OBJ) $(OBJADD) $(OBJBONUS) $(OBJPRINT)

FLAGS = -Wall -Wextra -Werror
CC = cc $(FLAGS)

all: $(NAME)

$(NAME): $(OBJALL)
		ar rcs $(NAME) $(OBJALL)

clean:
		rm -f $(OBJALL) a.out

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus e