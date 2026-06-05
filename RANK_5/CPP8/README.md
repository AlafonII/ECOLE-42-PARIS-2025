# CPP Module 08

> Rank 05 · École 42 Common Core — *C++ Piscine: templated containers, iterators, algorithms*

## What it does
Module 08 is where the **STL is finally allowed** — containers, iterators and
the `<algorithm>` header — and the exercises push you to use them.

## Exercises
- **ex00 — Easy find**: a template `easyfind` that locates a value inside any STL
  container of integers using STL algorithms, throwing if absent.
- **ex01 — Span**: a `Span` class that stores up to N numbers and reports the
  **shortest** and **longest** span (min/max distance) between any two of them,
  filling itself efficiently from an iterator range.
- **ex02 — Mutated abomination**: a `MutantStack` that adds **iterators** to
  `std::stack` (which normally has none) by inheriting from it and exposing the
  underlying container's iterators.

## How it works
The exercises lean on `std::vector`/`std::list`/`std::stack`, iterator ranges,
and algorithms like `std::find`, `std::min_element`/`std::max_element` and
`std::sort` instead of hand-rolled loops.

## Constraints (C++ Piscine, 2025)
- **C++98** only; `-Wall -Wextra -Werror -std=c++98`.
- The **STL is allowed and expected** in this module (use containers and
  algorithms as much as possible).
- No `using namespace`/`friend`; `*printf`/`*alloc`/`free` still forbidden.

## Build & run
```bash
cd ex01 && make && ./span
```

*Subject reference: 42 `CPP Module 08` subject (2024–2025 edition).*
