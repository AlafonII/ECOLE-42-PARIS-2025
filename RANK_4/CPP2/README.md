# CPP Module 02

> Rank 04 · École 42 Common Core — *C++ Piscine: ad-hoc polymorphism, operator overloading, OCF*

## What it does
Module 02 introduces the **Orthodox Canonical Form** (default ctor, copy ctor,
copy assignment, destructor) and **operator overloading**, built around a
fixed-point arithmetic class.

## Exercises
- **ex00** — a `Fixed` class storing a fixed-point number as raw bits, in
  Orthodox Canonical Form.
- **ex01** — add int/float conversion constructors, `toInt`/`toFloat`, and a
  `<<` stream-insertion operator.
- **ex02** — overload comparison (`> < >= <= == !=`), arithmetic
  (`+ - * /`), and increment/decrement operators, plus static `min`/`max`.

## How it works
The value is stored as an integer scaled by `2^8` (8 fractional bits). Conversions
shift accordingly; arithmetic operators convert to float, compute, and convert
back. Every class follows the canonical four-method form so copies and
assignments are well-defined.

## Constraints (C++ Piscine, 2025)
- **C++98** only; `-Wall -Wextra -Werror -std=c++98`.
- From this module on, classes must be written in **Orthodox Canonical Form**
  unless told otherwise.
- No `using namespace`/`friend`; **no STL**; `*printf`/`*alloc`/`free` forbidden.

## Build & run
```bash
cd ex02 && make && ./fixed
```

*Subject reference: 42 `CPP Module 02` subject (2024–2025 edition).*
