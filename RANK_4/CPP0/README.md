# CPP Module 00

> Rank 04 · École 42 Common Core — *C++ Piscine: namespaces, classes, member functions, I/O streams*

## What it does
The first C++ module introduces the language basics for someone coming from C:
namespaces, classes, member functions, `std::cout`/`std::cin` streams,
initialization lists, `static` and `const`.

## Exercises
- **ex00 — Megaphone**: echoes the program arguments back in UPPERCASE.
- **ex01 — PhoneBook**: a fixed-capacity (8 contacts) in-memory phone book with
  `ADD`, `SEARCH` and `EXIT` commands, demonstrating classes (`Contact`,
  `PhoneBook`), encapsulation and formatted column output.

## How it works
Each exercise is a self-contained program with its own `Makefile`. Data is held
in classes with private fields and public accessors; the PhoneBook stores
contacts in a circular fixed array (oldest overwritten once full) and prints a
truncated table view.

## Constraints (C++ Piscine, 2025)
- **C++98** standard only; compiled with `-Wall -Wextra -Werror -std=c++98`.
- Class design must follow the module's rules (no `using namespace`, no `friend`
  unless stated).
- **Forbidden**: C functions `*printf`, `*alloc`, `free` — use C++ streams and
  `new`/`delete` instead (use of them is graded **0**).
- The STL (containers/algorithms) is **not allowed** in this module.

## Build & run
```bash
cd ex00 && make && ./megaphone "hello"
cd ex01 && make && ./phonebook
```

*Subject reference: 42 `CPP Module 00` subject (2024–2025 edition).*
