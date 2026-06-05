# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alex <alex@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/29 10:05:12 by alafon            #+#    #+#              #
#    Updated: 2024/12/03 20:15:42 by alex             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a

SRC = ft_printf_utils.c ft_printf.c
OBJ = $(SRC:.c=.o)

FLAGS = -Wall -Wextra -Werror
CC = cc $(FLAGS)

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c
	$(CC) -c $< -o $@ 

clean:
		rm -f $(OBJ) a.out
		
fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re