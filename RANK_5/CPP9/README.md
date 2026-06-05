# CPP Module 09

> Rank 05 · École 42 Common Core — *C++ Piscine: the STL in anger*

## What it does
The final C++ module is a set of three standalone programs that must each be
built around a **different STL container**, applying everything from the piscine.

## Exercises
- **ex00 — BitcoinExchange**: reads a CSV price database and an input file of
  `date | value` lines, and prints the value multiplied by the bitcoin exchange
  rate **on (or closest before) that date** — a job for an associative container
  (`std::map`), with careful date/value validation.
- **ex01 — RPN**: evaluates a **Reverse Polish Notation** expression using a
  `std::stack`.
- **ex02 — PmergeMe**: sorts a sequence with the **Ford–Johnson
  (merge-insertion) algorithm**, implemented over **two different containers**
  (e.g. `std::vector` and `std::list`/`std::deque`), and times each to compare
  performance.

## How it works
Each program deliberately showcases a different container and the algorithmic
idea behind it: ordered lookup (map), LIFO evaluation (stack), and an optimal
comparison-sort (Ford–Johnson) whose timing is measured on each container.

## Constraints (C++ Piscine, 2025)
- **C++98** only; `-Wall -Wextra -Werror -std=c++98`.
- **STL allowed**, but each exercise must use a **different container** — and
  reusing the same container type across exercises is penalised.
- No `using namespace`/`friend`; `*printf`/`*alloc`/`free` forbidden; robust input
  validation with clear error messages is required.

## Build & run
```bash
cd ex00 && make && ./btc input.txt
cd ../ex02 && make && ./PmergeMe 3 5 9 7 4
```

*Subject reference: 42 `CPP Module 09` subject (2024–2025 edition).*
