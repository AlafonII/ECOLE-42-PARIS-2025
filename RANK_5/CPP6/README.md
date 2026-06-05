# CPP Module 06

> Rank 05 · École 42 Common Core — *C++ Piscine: C++ casts*

## What it does
Module 06 explores the four **C++ cast operators** and type identification:
`static_cast`, `reinterpret_cast`, `const_cast` and `dynamic_cast`.

## Exercises
- **ex00 — Conversion of scalar types**: a `ScalarConverter` that parses a string
  literal and converts it to `char`, `int`, `float` and `double`, handling
  special values (`nan`, `+inf`, `-inf`) and non-displayable / out-of-range cases.
- **ex01 — Serialization**: serialize a pointer to a `uintptr_t` and back with
  `reinterpret_cast`, proving the round-trip yields the same object.
- **ex02 — Real type identification**: generate a random `A`/`B`/`C` (all derived
  from `Base`) and identify the concrete type both with `dynamic_cast` on a
  pointer (using `NULL` on failure) and on a reference (using `catch`).

## How it works
Each cast is used for its intended purpose: `static_cast` for numeric/scalar
conversions, `reinterpret_cast` for the pointer↔integer round-trip, and
`dynamic_cast` for safe downcasting / runtime type identification.

## Constraints (C++ Piscine, 2025)
- **C++98** only; `-Wall -Wextra -Werror -std=c++98`.
- Orthodox Canonical Form where applicable; no `using namespace`/`friend`;
  **no STL**; `*printf`/`*alloc`/`free` forbidden.

## Build & run
```bash
cd ex00 && make && ./convert 42.0f
```

*Subject reference: 42 `CPP Module 06` subject (2024–2025 edition).*
