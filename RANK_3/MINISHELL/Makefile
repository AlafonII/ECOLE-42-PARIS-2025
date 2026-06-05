################################################################################
#                                Project Structure                             #
################################################################################

NAME := minishell
AUTHOR := jguillem, alafon
DATE := $(shell date +"%Y/%m/%d %T")

CC := clang

CFLAGS := -Wall -Wextra -Werror -I./include -I./libft/include
LFLAGS := -lreadline -L./libft -lft

SRC_DIR := src/
OBJ_DIR := obj/
INCLUDE_PATH := include/minishell.h
LIB := ./libft/libft.a

################################################################################
#                                 Source & Object                              #
################################################################################

BUILT_IN	:= $(addprefix built_in/, cd echo except pwd exit env export export_utils unset)
PARSING		:= $(addprefix parsing/, path pipeline pipeline_utils commands commands_utils exec_commands)
REDIRECT	:= $(addprefix redirect/, redirect redirect_analysis heredoc)
TOKENIZE	:= $(addprefix tokenize/, token token_utils expand expand_utils)
ERROR		:= $(addprefix error/, validity error)
SIGNAL		:= $(addprefix signal/, signal)
SYNTAX		:= $(addprefix syntax/, syntax)
FILES		:= main main_utils

ALL_FILES	:= $(FILES) $(BUILT_IN) $(PARSING) $(REDIRECT) $(TOKENIZE) $(ERROR) $(SIGNAL) $(SYNTAX)
SRC_PATHS	:= $(addprefix $(SRC_DIR), $(addsuffix .c, $(ALL_FILES)))
OBJ			:= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(ALL_FILES)))

################################################################################
#                                  Makefile rules                              #
################################################################################

$(NAME): $(OBJ) $(LIB)
	@$(call run_and_test,$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LFLAGS))

all: header $(NAME)

$(LIB):
	@$(call run_and_test_libft,$(MAKE) --no-print-directory --directory=./libft all)

$(OBJ_DIR)%.o : $(SRC_DIR)%.c $(HEADER)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/tokenize
	@mkdir -p $(OBJ_DIR)/built_in
	@mkdir -p $(OBJ_DIR)/parsing
	@mkdir -p $(OBJ_DIR)/redirect
	@mkdir -p $(OBJ_DIR)/error
	@mkdir -p $(OBJ_DIR)/signal
	@mkdir -p $(OBJ_DIR)/syntax
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
	@$(MAKE) CFLAGS="$(CFLAGS)" all

.PHONY: all clean fclean re header debug

################################################################################
#                                  Makefile visual                             #
################################################################################

.DEFAULT_GOAL := all

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

header:
	@printf "%b" "$(OK_COLOR)"
	@echo "                                    "
	@echo "             42424     242424 24242 "
	@echo "           42424       2424   24242 "
	@echo "         42424         24     24242 "
	@echo "       424242                 42424 "
	@echo "     424242                42424    "
	@echo "   424242                42424      "
	@echo " 424242                42424        "
	@echo " 424242424242424242    42424     24 "
	@echo " 424242424242424242    42424   2424 "
	@echo "              42424    24242 424242 "
	@echo "              42424                 "
	@echo "              42424                 "
	@printf "%b" "$(NO_COLOR)"
	@echo
	@printf "%b" "$(OK_COLOR)"
ifneq ($(HASH),)
	@printf "%b" "$(OBJ_COLOR)Name:\t$(WARN_COLOR)$(NAME)@$(HASH)\n"
else
	@printf "%b" "$(OBJ_COLOR)Name:\t$(WARN_COLOR)$(NAME)\n"
endif
	@printf "%b" "$(OBJ_COLOR)Author:\t$(WARN_COLOR)$(AUTHOR)\n"
	@printf "%b" "$(OBJ_COLOR)Date:\t$(WARN_COLOR)$(DATE)\n\033[m"
	@printf "%b" "$(OBJ_COLOR)CC:\t$(WARN_COLOR)$(CC)\n\033[m"
	@printf "%b" "$(OBJ_COLOR)Flags:\t$(WARN_COLOR)$(CFLAGS)\n\033[m"
	@echo
