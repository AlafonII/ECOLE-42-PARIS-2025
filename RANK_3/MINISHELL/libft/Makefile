CFLAGS := -Wall -Wextra -Werror

FILES := ft_isalpha \
		ft_isspace \
		ft_isonlyspace \
		ft_isdigit \
		ft_isalnum \
		ft_isascii \
		ft_isprint \
		ft_strlen \
		ft_memset \
		ft_bzero \
		ft_memcpy \
		ft_memmove \
		ft_strncpy \
		ft_strlcpy \
		ft_strlcat \
		ft_toupper \
		ft_tolower \
		ft_strchr	\
		ft_strrchr \
		ft_strcmp \
		ft_strncmp \
		ft_memchr \
		ft_memcmp \
		ft_strnstr \
		ft_atoi \
		ft_calloc \
		ft_strdup \
		ft_substr \
		ft_strjoin \
		ft_strtrim \
		ft_split \
		ft_itoa \
		ft_strmapi \
		ft_striteri \
		ft_putchar_fd \
		ft_putstr_fd \
		ft_putendl_fd \
		ft_putnbr_fd \
		ft_lstnew \
		ft_lstadd_front \
		ft_lstsize \
		ft_lstlast \
		ft_lstadd_back \
		ft_lstdelone \
		ft_lstclear \
		ft_lstiter \
		ft_lstmap

SRCS := $(addprefix src/, $(addsuffix .c, $(FILES)))
OBJS := $(addprefix obj/, $(addsuffix .o, $(FILES)))

NAME := libft.a

all: $(NAME)

$(NAME): $(OBJS)
	ar -rcs $(NAME) $(OBJS)

obj/%.o: src/%.c include/libft.h
	@mkdir -p obj
	cc $(CFLAGS) -c $< -I./include -o $@

clean:
	rm -f $(OBJS)
	rm -rf  obj

fclean: clean
	rm -f $(NAME)

re: fclean all
