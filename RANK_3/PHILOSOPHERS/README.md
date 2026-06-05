# Philosophers

> Rank 03 · École 42 Common Core — *The dining philosophers, with threads & mutexes*

## What it does
A simulation of the classic **dining philosophers** synchronization problem.
N philosophers sit around a table alternating between **eating**, **sleeping**
and **thinking**. There is one fork between each pair of neighbours, and a
philosopher needs **both adjacent forks** to eat. The program must keep everyone
fed without deadlock — and detect when a philosopher **starves to death**.

```
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

## How it works
- **One thread per philosopher** (`philosopher.c`, `init.c`) — each runs the
  eat → sleep → think loop independently.
- **Each fork is a mutex** — a philosopher must lock its two fork mutexes before
  eating; an ordering rule (even/odd pick-up order) prevents the classic
  deadlock where everyone grabs their left fork at once.
- **A monitoring thread** (`monitoring.c`) continuously checks every
  philosopher's "last meal" timestamp against `time_to_die`. If anyone exceeds
  it, the simulation prints the death and stops; it also stops once every
  philosopher has eaten the optional required number of times.
- **Timing** uses `gettimeofday()` for millisecond timestamps and a precise
  custom sleep built on `usleep`. A mutex also guards stdout so status lines
  (`is eating`, `is sleeping`, `died`, …) never interleave.

## Constraints (2025 subject)
- Written in C, **Norm-compliant**, compiled with `-Wall -Wextra -Werror`,
  **no global variables**, no data races / no memory leaks.
- Mandatory part uses **threads and mutexes** (the bonus uses **processes and
  semaphores** instead).
- Allowed functions: `pthread_create/join/detach`, `pthread_mutex_*`,
  `usleep`, `gettimeofday`, `malloc`, `free`, `write`, `printf`.
- A status message must be printed for every state change, prefixed with a
  timestamp in milliseconds, and must not mangle when printed concurrently.
- No philosopher should be able to "die" while the data says they ate in time —
  death detection must be tight (within a few ms).

## Build & run
```bash
make
./philo 5 800 200 200      # 5 philos; die after 800ms without eating
./philo 4 410 200 200 10   # stop once each has eaten 10 times
```

*Subject reference: 42 `philosophers` subject (2024–2025 edition).*
