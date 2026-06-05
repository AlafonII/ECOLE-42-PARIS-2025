# CPP Module 07

> Rank 05 · École 42 Common Core — *C++ Piscine: templates*

## What it does
Module 07 introduces **C++ templates** — writing functions and classes that work
for any type.

## Exercises
- **ex00 — Start with a few functions**: function templates `swap`, `min`, `max`
  that work on any comparable type.
- **ex01 — Iter**: a template `iter` applying a given function to every element
  of an array, regardless of element type.
- **ex02 — Array**: a template `Array<T>` class — a fixed-size, bounds-checked
  container that deep-copies, reports its `size()`, and throws
  `std::exception` on out-of-range access via `operator[]`.

## How it works
Generic code is written once with `template <typename T>` and instantiated by the
compiler per type. `Array<T>` manages its own heap storage with `new[]`/`delete[]`
and guards every index access.

## Constraints (C++ Piscine, 2025)
- **C++98** only; `-Wall -Wextra -Werror -std=c++98`.
- Templates must be defined in headers; no `using namespace`/`friend`.
- The STL is still **not allowed** here (that comes in Module 08);
  `*printf`/`*alloc`/`free` forbidden.

## Build & run
```bash
cd ex02 && make && ./array
```

*Subject reference: 42 `CPP Module 07` subject (2024–2025 edition).*
