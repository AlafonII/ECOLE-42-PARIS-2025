# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alex <alex@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/08 16:55:51 by alex              #+#    #+#              #
#    Updated: 2024/11/19 18:48:53 by alex             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRC = ft_strlen.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c\
ft_isprint.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c\
ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c\
ft_memcmp.c ft_strnstr.c ft_atoi.c ft_strlcat.c ft_strlcpy.c ft_calloc.c ft_strdup.c\

SRC_ADD = ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c\
ft_strmapi.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c\
ft_striteri.c ft_itoa.c

BONUS = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c\
ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c


OBJ = $(SRC:.c=.o)
OBJADD = $(SRC_ADD:.c=.o)
OBJBONUS = $(BONUS:.c=.o)
OBJALL = $(OBJ) $(OBJADD) $(OBJBONUS)

FLAGS = -Wall -Wextra -Werror
CC = cc $(FLAGS)

all: $(NAME)

$(NAME): $(OBJ) $(OBJADD)
		ar rcs $(NAME) $(OBJ) $(OBJADD)

bonus: $(OBJBONUS)
		ar rcs $(NAME) $(OBJBONUS)

e: $(OBJALL)
		ar rcs $(NAME) $(OBJALL)

clean:
		rm -f $(OBJALL) a.out

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus e