# Libft

> Rank 00 · École 42 Common Core — *Your first own C library*

## What it does
Libft is a static C library (`libft.a`) that re-implements a selection of the
standard C library, plus a handful of extra utility functions. Once built, it is
reused as a foundation in nearly every later C project of the cursus (FdF,
push_swap, minishell, cub3d, …).

## How it works
Each function lives in its own `.c` file and is prefixed with `ft_`. The
`Makefile` compiles them and bundles the resulting object files into an archive
with `ar`. Consumers include `libft.h` and link against `libft.a`.

The library is split into three families:
- **libc replicas** — memory (`ft_memset`, `ft_bzero`, `ft_memcpy`,
  `ft_memmove`, `ft_memchr`, `ft_memcmp`, `ft_calloc`), strings
  (`ft_strlen`, `ft_strlcpy`, `ft_strlcat`, `ft_strchr`, `ft_strrchr`,
  `ft_strnstr`, `ft_strncmp`, `ft_strdup`, `ft_atoi`) and character checks
  (`ft_isalpha`, `ft_isdigit`, `ft_isalnum`, `ft_isascii`, `ft_isprint`,
  `ft_toupper`, `ft_tolower`).
- **Additional helpers** — `ft_substr`, `ft_strjoin`, `ft_strtrim`,
  `ft_split`, `ft_itoa`, `ft_strmapi`, `ft_striteri`, and the `ft_put*_fd`
  output helpers.
- **Bonus — linked lists** — a `t_list` type with `ft_lstnew`,
  `ft_lstadd_front`, `ft_lstadd_back`, `ft_lstsize`, `ft_lstlast`,
  `ft_lstdelone`, `ft_lstclear`, `ft_lstiter`, `ft_lstmap`.

## Constraints (2025 subject)
- Written in C and **compliant with the Norm** (norminette).
- Every recoded libc function must keep the **same prototype and behaviour** as
  the original man page, and be prefixed with `ft_`.
- Compiled with `cc` and the flags `-Wall -Wextra -Werror`.
- The library must be produced with **`ar`** (using `libtool` is forbidden).
- No memory leaks; functions must behave as their originals on edge cases.
- The bonus part (linked-list functions) is only graded if the mandatory part
  is perfect, and lives in separate `_bonus` rules / files conventions.

## Build
```bash
make        # builds libft.a
make bonus  # adds the linked-list functions to the archive
make clean  # remove object files
make fclean # remove objects + libft.a
```

*Subject reference: 42 `libft` subject (2024–2025 edition).*
