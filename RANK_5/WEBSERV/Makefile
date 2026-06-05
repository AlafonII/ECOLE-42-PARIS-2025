################################################################################
#                                Project Structure                             #
################################################################################

CC      := c++
NAME    := webserv
AUTHOR  := alafon, jguillem
DATE    := $(shell date +"%Y/%m/%d %T")

SRC_DIR := src/
OBJ_DIR := obj/
INC_DIR := incl/

INCL    := -I$(INC_DIR) -I$(SRC_DIR)
CFLAGS  := -Wall -Werror -Wextra --std=c++98 $(INCL)
UPLOAD_DIR := www/site1/upload

################################################################################
#                                 Source & Object                              #
################################################################################

CORE	:= main
CONNECT	:= $(addprefix connect/, ServerSocket ClientConnection ClientConnection_Methods ClientConnection_Errors ClientConnection_Utils ConnectionManager)
HTTP	:= $(addprefix http/, HttpRequest HttpResponse)
ROUTER	:= $(addprefix router/, Router)
CONFIG	:= $(addprefix config/, ConfigParser)
CGI	:= $(addprefix cgi/, CGIHandler)

ALL_FILES  := $(CORE) $(CONNECT) $(HTTP) $(ROUTER) $(CONFIG) $(CGI)
SRC        := $(addprefix $(SRC_DIR), $(addsuffix .cpp, $(ALL_FILES)))
HDR        := $(addprefix $(INC_DIR), $(addsuffix .hpp, $(ALL_FILES)))
OBJ        := $(addprefix $(OBJ_DIR), $(addsuffix .o, $(ALL_FILES)))

################################################################################
#                                  Makefile rules                              #
################################################################################

.DEFAULT_GOAL := all

test:
	echo $(SRC)
all: header $(NAME)

$(NAME): $(OBJ) $(LIB) $(MLX)
	@$(call run_and_test,$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LFLAGS))

$(LIB):
	@$(call run_and_test_libft,$(MAKE) --no-print-directory --directory=./libft all)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(dir $@)
	@$(call run_and_test,$(CC) $(CFLAGS) -c $< -o $@)

clean: header
	@rm -f $(OBJ)
	@rm -rf $(OBJ_DIR)
	@printf "%-53b%b" "$(COM_COLOR)clean:" "$(OK_COLOR)[✓]$(NO_COLOR)\n"

fclean: header clean
	@rm -f $(NAME)
	@printf "%-53b%b" "$(COM_COLOR)fclean:" "$(OK_COLOR)[✓]$(NO_COLOR)\n"

re: fclean all

debug: CFLAGS += -g -gdwarf-2 -O0

debug: re

siege:
	@echo "Running siege for 10 seconds against http://127.0.0.1:8080/ ..."
	@siege -t10s http://127.0.0.1:8080/

remove:
	@true

.DEFAULT:
	@$(MAKE) --no-print-directory upload_message MSG="$@"

.PHONY: all clean fclean re header debug siege

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
	@echo "                           "
	@echo "        :::      ::::::::  "
	@echo "      :+:      :+:    :+:  "
	@echo "    +:+ +:+         +:+    "
	@echo "  +#+  +:+       +#+       "
	@echo "+#+#+#+#+#+   +#+          "
	@echo "     #+#    #+#            "
	@echo "    ###   ########.webserv "
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
