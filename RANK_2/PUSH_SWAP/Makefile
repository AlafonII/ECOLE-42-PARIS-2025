# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alex <alex@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/17 11:42:55 by alafon            #+#    #+#              #
#    Updated: 2025/03/01 12:50:45 by alex             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = push_swap

SRCS = main.c sort_utils/swap.c sort_utils/push.c sort_utils/rotate.c \
       sort_utils/rev_rotate.c

SORT_STACK = sorting/turk_sort_utils.c sorting/turk_sort.c \
			 sorting/small_sort.c sorting/sorting_utils.c

STACK = stack/stack_size.c stack/free_stack.c stack/stack_create.c

ERR = error_check/error_check.c error_check/error_check_utils.c

OBJ = $(SRCS:.c=.o) $(ERR:.c=.o) $(SORT_STACK:.c=.o) $(STACK:.c=.o)
OBJ_UNIQUE = $(sort $(OBJ))

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

FLAGS = -Wall -Wextra -Werror
CC = cc $(FLAGS) -I$(LIBFT_DIR)

OBJ_COUNT = 0

all: $(LIBFT) $(NAME)

$(LIBFT):
	@echo "Compiling libft... 🔧"
	@$(MAKE) -sC $(LIBFT_DIR) > /dev/null

$(NAME): $(OBJ)
	@echo "Linking $(NAME)... 🔧"
	@$(CC) -o $(NAME) $(OBJ) $(LIBFT) > /dev/null
	@echo "Compiled file $(words $(OBJ_UNIQUE))/$(words $(OBJ_UNIQUE)) 📄"
	@echo "✅ Compilation complete!"

%.o: %.c
	@$(eval TOTAL=$(words $(OBJ_UNIQUE)))
	@$(eval OBJ_COUNT=$(shell echo $$(( $(OBJ_COUNT) + 1 ))))
	@echo "Compiling file $(notdir $<) $(OBJ_COUNT)/$(TOTAL) 📄"
	@$(CC) -c $< -o $@ > /dev/null 2>&1
	@if [ "$(OBJ_COUNT)" -eq "$(TOTAL)" ]; then \
		echo "Compiled file $(TOTAL)/$(TOTAL) 📄"; \
	fi

clean:
	@rm -f $(OBJ) a.out
	@$(MAKE) clean -sC $(LIBFT_DIR) > /dev/null
	@echo "🧹 Cleaned object files."

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) fclean -sC $(LIBFT_DIR) > /dev/null
	@echo "🗑️  Full clean complete."

re: fclean all

.PHONY: all clean fclean re
