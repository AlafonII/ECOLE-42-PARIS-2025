# CPP Module 03

> Rank 04 · École 42 Common Core — *C++ Piscine: inheritance*

## What it does
Module 03 is about **inheritance** — deriving classes, reusing and extending base
behaviour, and the diamond problem it sets up for later.

## Exercises
- **ex00 — Aaaaand… OPEN!**: a base `ClapTrap` robot with HP, energy, attack.
- **ex01 — Serena, my love!**: `ScavTrap` **inherits** from `ClapTrap`,
  overriding behaviour and adding a special ability; demonstrates the
  construction/destruction chaining order.
- **ex02 — Repetitive work**: `FragTrap` also inherits from `ClapTrap`, setting
  up the multiple-inheritance scenario explored in Module 04.

## How it works
`ScavTrap` and `FragTrap` derive from `ClapTrap`, each calling the base
constructor and customising stats. Constructor/destructor messages make the
base→derived construction and derived→base destruction order visible.

## Constraints (C++ Piscine, 2025)
- **C++98** only; `-Wall -Wextra -Werror -std=c++98`.
- Classes in **Orthodox Canonical Form**; no `using namespace`/`friend`;
  **no STL**; `*printf`/`*alloc`/`free` forbidden.

## Build & run
```bash
cd ex02 && make && ./fragtrap
```

*Subject reference: 42 `CPP Module 03` subject (2024–2025 edition).*
