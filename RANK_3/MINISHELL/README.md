# Minishell

> Rank 03 · École 42 Common Core — *Writing a small Bash-like shell* (group project)

## What it does
Minishell is a simplified Unix shell. It shows a prompt, reads command lines,
parses them (with quoting, environment-variable expansion, redirections and
pipes), and executes built-ins or external programs found through `$PATH` —
mimicking the core behaviour of Bash.

## How it works
The source is organised as a small pipeline, mirrored by the `src/` sub-folders:

1. **`tokenize/`** — the raw line (read with **`readline`**) is split into tokens
   (words, operators `| < > >> <<`), respecting single and double quotes.
2. **`syntax/`** — the token stream is validated (unmatched quotes, misplaced
   pipes/redirections produce a syntax error).
3. **`parsing/`** — tokens are turned into a list of commands, each with its
   arguments, redirections, and `$VAR` / `$?` expansions resolved (single quotes
   suppress expansion, double quotes allow `$`).
4. **`redirect/`** — input/output redirections and here-documents (`<<`) are set
   up by duplicating file descriptors.
5. **execution** — pipelines are run by `fork()`ing each stage and wiring them
   together with `pipe()`; the **`built_in/`** commands (`echo`, `cd`, `pwd`,
   `export`, `unset`, `env`, `exit`) run in-process when needed.
6. **`signal/`** — interactive signal handling so `Ctrl-C`, `Ctrl-D` and
   `Ctrl-\` behave like Bash (new prompt, EOF, ignored, respectively).

A bundled **`libft/`** provides the low-level helpers; `include/` holds the
shared headers.

## Constraints (2025 subject)
- Written in C, **Norm-compliant**, compiled with `-Wall -Wextra -Werror`,
  **no memory leaks** in your own code, **one global** allowed *only* to store
  a signal number (nothing else).
- Must use **`readline`** (and may use its history). Allowed functions are a long
  but fixed list: `readline`, `add_history`, `printf`, `malloc`, `free`,
  `write`, `access`, `open`, `read`, `close`, `fork`, `wait*`, `execve`, `dup`,
  `dup2`, `pipe`, `signal`/`sigaction`, `kill`, the `*dir` family, `stat`/`lstat`,
  `getcwd`, `chdir`, `unlink`, `getenv`, `tcsetattr`/`tcgetattr`, `isatty`,
  `ttyname`, the termcap functions, etc.
- Mandatory features: prompt, command history, executable lookup via `$PATH` or
  absolute/relative path, single & double quotes, redirections `< > >> <<`,
  pipes `|`, `$VAR` and `$?` expansion, the seven built-ins above, and correct
  signal handling.
- **Not required** in the mandatory part: `;`, `&&`, `||`, `\`, wildcards
  (those are bonus).

## Build & run
```bash
make
./minishell
```

*Group project (co-authored). Subject reference: 42 `minishell` subject (2024–2025 edition).*
