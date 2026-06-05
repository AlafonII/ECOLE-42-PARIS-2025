# get_next_line

> Rank 01 · École 42 Common Core — *Reading a line from a file descriptor*

## What it does
`get_next_line()` returns the next line (including the trailing `\n`, if any)
from a file descriptor on each call. Calling it repeatedly walks through an
entire file — or stdin, or any readable fd — one line at a time, until it returns
`NULL` at end of file.

```c
char *get_next_line(int fd);
```

## How it works
The function reads the fd in chunks of `BUFFER_SIZE` bytes using `read()` and
accumulates the bytes in a **static variable** that survives between calls. On
each call it:
1. reads more chunks until a newline is present in the stash (or EOF is hit),
2. extracts everything up to and including the first `\n` as the returned line,
3. keeps the remainder in the static stash for the following call.

Files in this folder:
- `get_next_line.c` / `get_next_line.h` — mandatory version (single fd).
- `get_next_line_utils.c` — helper functions (length, join, copy…).
- `get_next_line_bonus.c` / `_bonus.h` / `_utils_bonus.c` — bonus version that
  tracks a **separate static stash per file descriptor**, so multiple fds can be
  read in an interleaved fashion without mixing their data.

## Constraints (2025 subject)
- Written in C, **Norm-compliant**, compiled with `-Wall -Wextra -Werror`.
- The read buffer size is a compile-time macro named **`BUFFER_SIZE`**, passed
  via `-D BUFFER_SIZE=n`; the code must work for *any* value (1, 42, 9999, …).
- **`lseek` is forbidden**, and so is reading the whole file in one go — you must
  use repeated `read()` calls bounded by `BUFFER_SIZE`.
- **Global variables are forbidden** — state is kept in `static` variables only.
- Allowed functions: `read`, `malloc`, `free`. Behaviour on binary files or
  `read` errors is undefined (you only need defined behaviour on text fds).
- Mandatory part: one static variable. Bonus: a single static variable handling
  multiple fds simultaneously.

## Build / try it
There is no Makefile (the project is meant to be compiled into a tester):
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   get_next_line.c get_next_line_utils.c your_main.c -o gnl
```

*Subject reference: 42 `get_next_line` subject (2024–2025 edition).*
