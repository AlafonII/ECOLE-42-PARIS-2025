# CPP Module 01

> Rank 04 · École 42 Common Core — *C++ Piscine: memory, references, pointers to members*

## What it does
Module 01 covers heap allocation with `new`/`delete`, the difference between
references and pointers, pointers to member functions, and the `switch`
statement.

## Exercises
- **ex00 — Zombie**: `newZombie` (heap) vs `randomChump` (stack) — object
  lifetime and when to allocate.
- **ex01 — Moar Brainz!**: allocate an array of `Zombie` in one `new[]`.
- **ex02 — HI THIS IS BRAIN**: print a string, a pointer to it and a reference
  to it, to show they share the same address.
- **ex03 — Violence**: `Weapon`, `HumanA` (holds a **reference**) and `HumanB`
  (holds a **pointer**) — when each is appropriate.
- **ex04 — Sed is for losers**: a file find-and-replace tool using C++ string
  streams instead of `sed`.
- **ex05 — Harl 2.0**: log levels dispatched through an array of **pointers to
  member functions**.

## Constraints (C++ Piscine, 2025)
- **C++98** only; `-Wall -Wextra -Werror -std=c++98`.
- No `using namespace`/`friend` unless stated; **no STL** in this module.
- **`*printf`, `*alloc`, `free` are forbidden** (graded 0); memory comes from
  `new`/`delete` and every allocation must be freed.

## Build & run
```bash
cd ex03 && make && ./violence
```

*Subject reference: 42 `CPP Module 01` subject (2024–2025 edition).*
