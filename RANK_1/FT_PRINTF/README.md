# ft_printf

> Rank 01 · École 42 Common Core — *Recoding printf with variadic arguments*

## What it does
`ft_printf()` is a re-implementation of the C standard `printf()`. It writes
formatted output to the standard output and returns the number of characters
printed, mimicking the behaviour of the original for the supported conversions.

```c
int ft_printf(const char *format, ...);
```

## How it works
The function uses the **`<stdarg.h>`** variadic API (`va_start`, `va_arg`,
`va_end`) to walk the argument list. It scans the format string character by
character; on each `%` it reads the following conversion specifier and dispatches
to the matching handler, which converts the argument to text and writes it with
`write()`, accumulating the total count of printed characters.

Files:
- `ft_printf.c` — main parsing/dispatch loop.
- `ft_printf_utils.c` — per-conversion output helpers (numbers, hex, pointers…).
- `ft_printf.h` — prototypes.

Supported conversions: **`c s p d i u x X %`**
- `c` character, `s` string, `p` pointer (hex with `0x`),
- `d`/`i` signed decimal, `u` unsigned decimal,
- `x`/`X` hexadecimal (lower/upper case), `%` literal percent.

## Constraints (2025 subject)
- Written in C, **Norm-compliant**, compiled with `-Wall -Wextra -Werror`.
- Must manage the conversions `cspdiuxX%` and behave like the real `printf`
  for them.
- The library is built with **`ar`** (using `libtool` is forbidden).
- Global variables are forbidden.
- The bonus part adds flag handling (`-0.# +` and space, field width and
  precision); the mandatory part does not require flags.

## Build
```bash
make        # builds libftprintf.a
make bonus  # builds with the flag-handling bonus
```

*Subject reference: 42 `ft_printf` subject (2024–2025 edition).*
