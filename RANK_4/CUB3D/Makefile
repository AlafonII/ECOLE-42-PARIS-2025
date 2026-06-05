################################################################################
#                                Project Structure                             #
################################################################################

CC      := clang
NAME    := cub3D
AUTHOR  := jguillem, alafon
DATE    := $(shell date +"%Y/%m/%d %T")

SRC_DIR := src/
OBJ_DIR := obj/
INC_DIR := incl/

LFLAGS  := -Lmlx -lmlx -lXext -lX11 -lm -L./libft -lft
INCL    := -I$(INC_DIR) -Imlx -I/usr/include/X11 -I./libft/include -I./libft/get_next_line
CFLAGS  := -Wall -Werror -Wextra $(INCL)

LIB     := ./libft/libft.a
MLX_DIR := mlx
MLX     := $(MLX_DIR)/libmlx.a

################################################################################
#                                 Source & Object                              #
################################################################################

MAP_CHECK	:= $(addprefix map_check/, map_file_check player_check map_fill map_check)
BFS			:= $(addprefix bfs/, bfs_context bfs_map bfs_utils bfs)
PARSING		:= $(addprefix parsing/, parse_data parse_map parse_texture parse_colour parse_texture_utils)
ERROR		:= $(addprefix error/, error)
FREE		:= $(addprefix free/, free)
INIT		:= $(addprefix initialise/, init init_utils)
DISPLAY		:= $(addprefix display/, mlx image textures raycasting)
GAMING		:= $(addprefix gaming/, player_movement try_move)
FILES		:= cub3d

ALL_FILES  := $(FILES) $(MAP_CHECK) $(BFS) $(PARSING) $(ERROR) $(FREE) $(INIT) $(DISPLAY) $(GAMING)
SRC        := $(addprefix $(SRC_DIR),$(addsuffix .c,$(ALL_FILES)))
HDR        := $(addprefix $(INC_DIR),$(addsuffix .h,$(ALL_FILES)))
OBJ        := $(addprefix $(OBJ_DIR),$(addsuffix .o,$(ALL_FILES)))

################################################################################
#                                  Makefile rules                              #
################################################################################

.DEFAULT_GOAL := all

all: header $(NAME)

$(NAME): $(OBJ) $(LIB) $(MLX)
	@$(call run_and_test,$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LFLAGS))

$(LIB):
	@$(call run_and_test_libft,$(MAKE) --no-print-directory --directory=./libft all)

$(MLX):
	@$(call run_and_test_mlx,$(MAKE) --no-print-directory --directory=$(MLX_DIR) all)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	@$(call run_and_test,$(CC) $(CFLAGS) -c $< -o $@)

clean: header
	@rm -f $(OBJ)
	@rm -rf $(OBJ_DIR)
	@$(MAKE) --no-print-directory --directory=./libft clean > /dev/null 2>&1
	@printf "%-53b%b" "$(COM_COLOR)clean:" "$(OK_COLOR)[✓]$(NO_COLOR)\n"

fclean: header clean
	@rm -f $(NAME)
	@$(MAKE) --no-print-directory --directory=./libft fclean > /dev/null 2>&1
	@printf "%-53b%b" "$(COM_COLOR)fclean:" "$(OK_COLOR)[✓]$(NO_COLOR)\n"

re: fclean all

debug: fclean
debug: CFLAGS += -g -gdwarf-2
debug:
	@$(MAKE) CFLAGS="$(CFLAGS)" --directory=./libft all
	@$(MAKE) --directory=$(MLX_DIR) all
	@$(MAKE) CFLAGS="$(CFLAGS)" all

.PHONY: all clean fclean re header debug

################################################################################
#                                  Makefile visual                             #
################################################################################

COM_COLOR   = \033[0;34m
OBJ_COLOR   = \033[0;36m
OK_COLOR    = \033[0;32m
ERROR_COLOR = \033[0;31m
WARN_COLOR  = \033[0;33m
NO_COLOR    = \033[m

COM_STRING  = "compiling"

define run_and_test
	printf "%b%-46b" "$(COM_COLOR)$(COM_STRING) " "$(OBJ_COLOR)$(@F)$(NO_COLOR)"; \
	$(1); \
	RESULT=$$?; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%b\n" "$(ERROR_COLOR)[✖]$(NO_COLOR)"; \
	else  \
		printf "%b\n" "$(OK_COLOR)[✓]$(NO_COLOR)"; \
	fi; \
	exit $$RESULT
endef

define run_and_test_libft
	printf "%b%-46b" "$(COM_COLOR)$(COM_STRING) " "$(OBJ_COLOR)libft$(NO_COLOR)"; \
	OUTPUT=$$({ $(1); } 2>&1); \
	RESULT=$$?; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%b\n" "$(ERROR_COLOR)[✖]$(NO_COLOR)"; \
		echo "$$OUTPUT"; \
	else  \
		printf "%b\n" "$(OK_COLOR)[✓]$(NO_COLOR)"; \
	fi; \
	exit $$RESULT
endef

define run_and_test_mlx
	printf "%b%-46b" "$(COM_COLOR)$(COM_STRING) " "$(OBJ_COLOR)mlx$(NO_COLOR)"; \
	OUTPUT=$$({ $(1); } 2>&1); \
	RESULT=$$?; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%b\n" "$(ERROR_COLOR)[✖]$(NO_COLOR)"; \
		echo "$$OUTPUT"; \
	else  \
		printf "%b\n" "$(OK_COLOR)[✓]$(NO_COLOR)"; \
	fi; \
	exit $$RESULT
endef

header:
	@printf "%b" "$(OBJ_COLOR)"
	@echo "                          "
	@echo "        :::      :::::::: "
	@echo "      :+:      :+:    :+: "
	@echo "    +:+ +:+         +:+   "
	@echo "  +#+  +:+       +#+      "
	@echo "+#+#+#+#+#+   +#+         "
	@echo "     #+#    #+#           "
	@echo "    ###   ########.cub3D  "
	@printf "%b" "$(NO_COLOR)"
	@echo
	@printf "%b" "$(OK_COLOR)"
ifneq ($(HASH),)
	@printf "%b" "$(OBJ_COLOR)Name:	$(WARN_COLOR)$(NAME)@$(HASH)\n"
else
	@printf "%b" "$(OBJ_COLOR)Name:	$(WARN_COLOR)$(NAME)\n"
endif
	@printf "%b" "$(OBJ_COLOR)Author:	$(WARN_COLOR)$(AUTHOR)\n"
	@printf "%b" "$(OBJ_COLOR)Date:	$(WARN_COLOR)$(DATE)\n\033[m"
	@printf "%b" "$(OBJ_COLOR)CC:	$(WARN_COLOR)$(CC)\n\033[m"
	@printf "%b" "$(OBJ_COLOR)Flags:	$(WARN_COLOR)$(CFLAGS)\n\033[m"
	@echo
	@printf "%b" "$(NO_COLOR)"
