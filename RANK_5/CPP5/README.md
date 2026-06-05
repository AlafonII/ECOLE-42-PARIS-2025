# CPP Module 05

> Rank 05 · École 42 Common Core — *C++ Piscine: repetition and exceptions*

## What it does
Module 05 is about **exception handling** (`try`/`catch`, custom exception
classes) and how exceptions propagate, built around a bureaucracy theme.

## Exercises
- **ex00 — Mommy, when I grow up…**: a `Bureaucrat` with a name and a grade
  (1–150) that throws `GradeTooHigh`/`GradeTooLow` exceptions on invalid grades.
- **ex01 — Form up, maggots!**: a `Form` requiring grades to be signed/executed;
  signing throws when the bureaucrat's grade is insufficient.
- **ex02 — No, you need form 28B…**: an abstract `AForm` with concrete
  `ShrubberyCreationForm`, `RobotomyRequestForm`, `PresidentialPardonForm`, each
  with its own execution effect and grade requirements.
- **ex03 — At least this beats coffee-making**: an `Intern` that builds the right
  form from a name string (a small factory), without giant if/else chains.

## How it works
Invalid states are signalled by **throwing** nested exception classes; callers
wrap operations in `try`/`catch`. Forms validate the acting bureaucrat's grade
before signing and before executing, demonstrating controlled error propagation.

## Constraints (C++ Piscine, 2025)
- **C++98** only; `-Wall -Wextra -Werror -std=c++98`.
- Orthodox Canonical Form; no `using namespace`/`friend`; **no STL**;
  `*printf`/`*alloc`/`free` forbidden. Errors must use exceptions, not return codes.

## Build & run
```bash
cd ex02 && make && ./bureaucrat
```

*Subject reference: 42 `CPP Module 05` subject (2024–2025 edition).*
