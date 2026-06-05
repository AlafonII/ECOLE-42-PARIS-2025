# push_swap

> Rank 02 · École 42 Common Core — *Sorting integers with two stacks and a limited instruction set*

## What it does
`push_swap` sorts a list of integers received as arguments using **only two
stacks (`a` and `b`)** and a **restricted set of operations**, while trying to
output **as few operations as possible**. It prints the sequence of operations
(one per line) that sorts the stack; a companion `checker` can replay that
sequence to verify correctness.

## Allowed operations
| op | effect |
|----|--------|
| `sa` / `sb` / `ss` | swap the top two elements of a / b / both |
| `pa` / `pb` | push the top of one stack onto the other |
| `ra` / `rb` / `rr` | rotate a / b / both up (top goes to bottom) |
| `rra` / `rrb` / `rrr` | reverse-rotate a / b / both down |

## How it works
- **Parsing & validation** (`error_check/`) — arguments are parsed into integers,
  rejecting non-numbers, overflow, and duplicates with `Error\n` on stderr.
- **Small sorts** (`sorting/small_sort.c`) — hard-coded optimal routines for
  2, 3, 4 and 5 elements.
- **Chunk sort** (`sorting/turk_sort.c`) — the general case. Values are indexed
  (rank-compressed), then pushed from `a` to `b` in **chunks** based on their
  index range (`compute_chunk_count` / `compute_chunk_size`); the algorithm picks
  cheap moves to fill `b`, then pushes everything back to `a` in order. This is
  the well-known "chunk/Turkish" strategy, which keeps the operation count within
  the grading thresholds.
- **Stack & op primitives** live in `stack/` and `sort_utils/`.

## Constraints (2025 subject)
- Written in C, **Norm-compliant**, compiled with `-Wall -Wextra -Werror`.
- Allowed functions: **`read`, `write`, `malloc`, `free`, `exit`** (plus your
  libft). No other libc helpers.
- Only the operations above may be used; instruction names must be exact.
- Invalid input must print `Error` followed by a newline to **stderr**.
- Performance targets (mandatory): **≤ 3 operations for 3 numbers**,
  **≤ 12 for 5 numbers**; for 100 numbers a good score is < ~700 ops and for
  500 numbers < ~5500 ops.
- Bonus: a separate `checker` program that reads operations on stdin and prints
  `OK`/`KO`.

## Build & run
```bash
make
./push_swap 2 1 3 6 5 8        # prints the operations
ARG="4 67 3 87 23"; ./push_swap $ARG | ./checker_linux $ARG   # verify (if checker present)
```

*Subject reference: 42 `push_swap` subject (2024–2025 edition).*
