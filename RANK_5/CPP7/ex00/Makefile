################################################################################
#                                Project Structure                             #
################################################################################

NAME     := ex00
AUTHOR   := alafon
DATE     := $(shell date +"%Y/%m/%d %T")

CC       := c++
CFLAGS   := -Wall -Wextra -Werror -std=c++98

SRC_DIR  := src/
OBJ_DIR  := obj/

################################################################################
#                                 Source & Object                              #
################################################################################

# ALL_FILES :=

SRC_PATHS := main.cpp \
             $(addprefix $(SRC_DIR), $(addsuffix .cpp, $(ALL_FILES)))

OBJ := $(addprefix $(OBJ_DIR), main.o) \
       $(addprefix $(OBJ_DIR), $(addsuffix .o, $(ALL_FILES)))

################################################################################
#                                  Makefile rules                              #
################################################################################

$(NAME): $(OBJ)
	@$(call run_and_test,$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LFLAGS))

all: header $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(call run_and_test,$(CC) $(CFLAGS) -c $< -o $@)

$(OBJ_DIR)main.o: main.cpp
	@mkdir -p $(OBJ_DIR)
	@$(call run_and_test,$(CC) $(CFLAGS) -c $< -o $@)

clean: header
	@rm -f $(OBJ)
	@rm -rf $(OBJ_DIR)
	@printf "%-53b%b" "$(COM_COLOR)clean:" "$(OK_COLOR)[✓]$(NO_COLOR)\n"

fclean: header clean
	@rm -f $(NAME)
	@printf "%-53b%b" "$(COM_COLOR)fclean:" "$(OK_COLOR)[✓]$(NO_COLOR)\n"

re: fclean all

.PHONY: all clean fclean re header

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

header:
	@printf "%b" "$(OBJ_COLOR)"
	@echo "                          "
	@echo "        :::      :::::::: "
	@echo "      :+:      :+:    :+: "
	@echo "    +:+ +:+         +:+   "
	@echo "  +#+  +:+       +#+      "
	@echo "+#+#+#+#+#+   +#+         "
	@echo "     #+#    #+#           "
	@echo "    ###   ########.C07.ex00"
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
