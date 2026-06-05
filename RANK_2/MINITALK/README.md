# Minitalk

> Rank 02 · École 42 Common Core — *Client/server communication using UNIX signals*

## What it does
Minitalk is a small messaging system made of two programs: a **server** and a
**client**. The client sends a string to the server using nothing but **UNIX
signals**, and the server prints it as it arrives.

## How it works
Only two signals are available: **`SIGUSR1`** and **`SIGUSR2`**. They are used as
a single bit each:
- the **client** (`client.c`) takes the server PID and a string, then transmits
  each character **bit by bit** — sending `SIGUSR1` for a `0` and `SIGUSR2` for a
  `1` via `kill()`, with a tiny pause between bits;
- the **server** (`server.c`) installs a `sigaction` handler that reconstructs
  each byte one bit at a time, and once 8 bits are collected, writes the
  character. A null byte marks the end of the message.

`utils.c` holds output helpers and `minitalk.h` the shared declarations. This
implementation also uses `siginfo_t` to recover the client's PID, enabling an
**acknowledgement** back to the client (the bonus behaviour).

## Constraints (2025 subject)
- Written in C, **Norm-compliant**, compiled with `-Wall -Wextra -Werror`.
- The server prints its **PID** on launch; communication uses **only `SIGUSR1`
  and `SIGUSR2`**.
- The client takes exactly two parameters: the **server PID** and the **string**.
- No memory leaks; signals must not be lost (reliable handling with `sigaction`
  is expected, and the server should handle a reasonably fast client).
- Allowed functions: `write`, `signal`/`sigaction`, `sigemptyset`,
  `sigaddset`, `kill`, `getpid`, `malloc`, `free`, `pause`, `sleep`, `usleep`,
  `exit` (plus your libft).
- Bonus: the server **acknowledges** every received message, and Unicode
  characters are supported.

## Build & run
```bash
make
./server                 # prints: Server PID: <pid>
./client <pid> "hello"   # in another terminal
```

*Subject reference: 42 `minitalk` subject (2024–2025 edition).*
