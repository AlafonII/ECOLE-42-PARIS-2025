# CPP Module 04

> Rank 04 · École 42 Common Core — *C++ Piscine: subtype polymorphism, abstract classes, interfaces*

## What it does
Module 04 covers **runtime polymorphism**: virtual functions, abstract base
classes, and deep copying.

## Exercises
- **ex00 — Polymorphism**: `Animal` base with `Dog`/`Cat` overriding
  `makeSound()` via a **virtual** method; `WrongAnimal`/`WrongCat` show what
  happens *without* `virtual`.
- **ex01 — I don't want to set the world on fire**: each `Dog`/`Cat` owns a
  `Brain` allocated on the heap — copying must **deep-copy** the brain, not just
  the pointer.
- **ex02 — Abstract class**: `AAnimal` is made **abstract** (pure virtual
  method) so it can no longer be instantiated directly.

## How it works
Base classes declare a virtual `makeSound()`; derived classes override it, so a
`Animal*` pointing at a `Dog` barks. Deep copies allocate a new `Brain` and copy
its contents, preventing double-free and shared-state bugs.

## Constraints (C++ Piscine, 2025)
- **C++98** only; `-Wall -Wextra -Werror -std=c++98`.
- Orthodox Canonical Form; no `using namespace`/`friend`; **no STL**;
  `*printf`/`*alloc`/`free` forbidden. No memory leaks (deep copy required).

## Build & run
```bash
cd ex02 && make && ./animal
```

*Subject reference: 42 `CPP Module 04` subject (2024–2025 edition).*
