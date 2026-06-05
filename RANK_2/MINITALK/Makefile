# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alafon <alafon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/27 17:11:57 by alex              #+#    #+#              #
#    Updated: 2025/02/03 14:15:12 by alafon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC          = cc
CFLAGS      = -Wall -Wextra -Werror
RM          = rm -f

NAME_SERVER = server
NAME_CLIENT = client

SRC_SERVER  = server.c utils.c
SRC_CLIENT  = client.c utils.c

OBJ_SERVER  = $(SRC_SERVER:.c=.o)
OBJ_CLIENT  = $(SRC_CLIENT:.c=.o)
OBJ_ALL     = $(sort $(OBJ_SERVER) $(OBJ_CLIENT))

OBJ_COUNT   = 0

all: $(NAME_SERVER) $(NAME_CLIENT)

$(NAME_SERVER): $(OBJ_SERVER)
	@echo "Linking $(NAME_SERVER)... 🔧"
	@$(CC) $(CFLAGS) $^ -o $@ > /dev/null
	@echo "✅ $(NAME_SERVER) compiled!"

$(NAME_CLIENT): $(OBJ_CLIENT)
	@echo "Linking $(NAME_CLIENT)... 🔧"
	@$(CC) $(CFLAGS) $^ -o $@ > /dev/null
	@echo "✅ $(NAME_CLIENT) compiled!"

%.o: %.c
	@$(eval TOTAL=$(words $(OBJ_ALL))) # Get total unique object count
	@$(eval OBJ_COUNT=$(shell echo $$(( $(OBJ_COUNT) + 1 )))) # Increment counter
	@echo "Compiling file $(notdir $<) $(OBJ_COUNT)/$(TOTAL) 📄"
	@$(CC) $(CFLAGS) -c $< -o $@ > /dev/null
	@if [ "$(OBJ_COUNT)" -eq "$(TOTAL)" ]; then echo "Total compiled file $(TOTAL)/$(TOTAL) 📄"; fi

clean:
	@$(RM) $(OBJ_SERVER) $(OBJ_CLIENT)
	@echo "🧹 Cleaned object files."

fclean: clean
	@$(RM) $(NAME_SERVER) $(NAME_CLIENT)
	@echo "🗑️  Full clean complete."

re: fclean all

.PHONY: all clean fclean re
