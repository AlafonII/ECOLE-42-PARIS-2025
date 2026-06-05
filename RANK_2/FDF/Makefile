# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alafon <alafon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/31 14:32:16 by alex              #+#    #+#              #
#    Updated: 2025/02/24 09:14:13 by alafon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# =======================
#       COMPILER
# =======================
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g3

# =======================
#     DIRECTORIES
# =======================
LIB_DIR   = libraries
MLX_DIR   = $(LIB_DIR)/minilibx-linux
LIBFT_DIR = $(LIB_DIR)/libft

# =======================
#       LIBRARIES
# =======================
MLX_LIB   = $(MLX_DIR)/libmlx_Linux.a
LIBFT_LIB = $(LIBFT_DIR)/libft.a

# =======================
#       INCLUDES
# =======================
INCLUDES  = -I $(MLX_DIR) -I $(LIBFT_DIR)

# =======================
#     LINKER FLAGS
# =======================
LDFLAGS   = -L $(MLX_DIR) -lmlx_Linux -L $(LIBFT_DIR) -lft -lXext -lX11 -lbsd -lm

# =======================
#    SOURCE & OBJECTS
# =======================
SRC       = fdf.c src/fdf_read.c src/iso_projection.c src/draw.c\
			src/init_mlx.c src/hooks.c src/cleanup.c src/iso_projection_utils.c \
			src/parse_fdf.c src/error.c
OBJ       = $(SRC:.c=.o)
OBJ_COUNT = 0

# =======================
#      EXECUTABLE
# =======================
NAME      = fdf

# =======================
#        RULES
# =======================

all: $(LIBFT_LIB) $(MLX_LIB) $(NAME)

$(LIBFT_LIB):
	@echo "Compiling libft... 🔧"
	@$(MAKE) -C $(LIBFT_DIR) > /dev/null

$(MLX_LIB):
	@echo "Compiling minilibx... 🔧"
	@$(MAKE) -C $(MLX_DIR) > /dev/null

$(NAME): $(OBJ) $(LIBFT_LIB) $(MLX_LIB)
	@echo "Linking executable... 🔧"
	@$(CC) $(CFLAGS) $(OBJ) $(INCLUDES) $(LDFLAGS) -o $(NAME) > /dev/null
	@echo "Compiled file $(words $(OBJ))/$(words $(OBJ)) 📄"
	@echo "✅ Compilation complete!"

%.o: %.c
	@$(eval OBJ_COUNT=$(shell echo $$(( $(OBJ_COUNT) + 1 )))) # Increment counter
	@echo "Compiling file $(notdir $<) $(OBJ_COUNT)/$(words $(OBJ)) 📄"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ > /dev/null

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean > /dev/null
	@$(MAKE) -C $(MLX_DIR) clean > /dev/null
	@rm -f $(OBJ)
	@echo "🧹 Cleaned object files."

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean > /dev/null
	@$(MAKE) -C $(MLX_DIR) clean > /dev/null
	@rm -f $(NAME)
	@echo "🗑️  Full clean complete."

re: fclean all

.PHONY: all clean fclean re

