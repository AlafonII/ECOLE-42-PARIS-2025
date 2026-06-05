# École 42 Paris — Common Core Archive

An archive of the projects I completed during the **Common Core** at
**École 42 Paris** (2024–2026). Every project lives here as plain source — the
code was cloned out of its original 42 repository and committed directly into this
monorepo (no submodules), then lightly cleaned up for readability and documented
with its own README.

The cursus is overwhelmingly **C**, moves into **C++** (the "C++ Piscine",
modules 00–09), touches **system administration**, **networking**, **Docker**, and
finishes with a full-stack **TypeScript** web application.

---

## How the 42 rank system works

The Common Core has no classes and no teachers. It is a sequence of projects
grouped into **ranks (00 → 06)**. You start at rank 00, and completing a rank's
projects earns the experience needed to unlock the next rank. The difficulty and
scope grow at each step: from a personal C library, through algorithms and Unix
systems programming, up to graphics, web servers, infrastructure, and a final
group web project.

This repository mirrors that structure directly:

```
<RANK>/<PROJECT>/      e.g.  RANK_2/PUSH_SWAP/   RANK_4/CUB3D/
```

Each `RANK_*` folder is a rank; each sub-folder is one project, containing its
source files and a `README.md` describing what it does, how it works, and the
2025-era subject constraints.

> A couple of projects are **not** programming projects, so their folders hold no
> source: **Born2beRoot** (a configured virtual machine — only its disk signature
> is committed) and **NetPractice** (a networking simulator — only the solved
> level configs are committed). Each explains this in its own README.

Some projects were **group projects** (co-authored): Minishell, cub3D, webserv and
ft_transcendence.

---

## Index

### 🟢 RANK 00 — Your first C library
| Project | Language | What it is |
|---------|----------|------------|
| [LIBFT](RANK_0/LIBFT) | C | A static library re-implementing libc functions + extras, reused everywhere after. |

### 🟢 RANK 01 — Foundations & system intro
| Project | Language | What it is |
|---------|----------|------------|
| [BORN2BEROOT](RANK_1/BORN2BEROOT) | SysAdmin | Harden & configure a Debian VM (LVM, SSH, UFW, sudo, monitoring). *No code — signature only.* |
| [GET_NEXT_LINE](RANK_1/GET_NEXT_LINE) | C | Read a file descriptor line by line using static memory. |
| [FT_PRINTF](RANK_1/FT_PRINTF) | C | Recode `printf` with variadic arguments. |

### 🟡 RANK 02 — Algorithms & graphics intro
| Project | Language | What it is |
|---------|----------|------------|
| [FDF](RANK_2/FDF) | C | 3D wireframe renderer (isometric projection) with MiniLibX. |
| [PUSH_SWAP](RANK_2/PUSH_SWAP) | C | Sort integers with two stacks and a limited instruction set, in minimal moves. |
| [MINITALK](RANK_2/MINITALK) | C | Client/server messaging using only UNIX signals. |

### 🟡 RANK 03 — Unix processes & threads
| Project | Language | What it is |
|---------|----------|------------|
| [MINISHELL](RANK_3/MINISHELL) | C | A small Bash-like shell (pipes, redirections, quoting, builtins). *Group project.* |
| [PHILOSOPHERS](RANK_3/PHILOSOPHERS) | C | The dining-philosophers problem with threads & mutexes. |

### 🟠 RANK 04 — C++ Piscine, graphics & networking
| Project | Language | What it is |
|---------|----------|------------|
| [CPP0](RANK_4/CPP0) | C++ | Namespaces, classes, I/O streams. |
| [CPP1](RANK_4/CPP1) | C++ | Memory, references, pointers to members. |
| [CPP2](RANK_4/CPP2) | C++ | Operator overloading & Orthodox Canonical Form. |
| [CPP3](RANK_4/CPP3) | C++ | Inheritance. |
| [CPP4](RANK_4/CPP4) | C++ | Polymorphism, abstract classes, interfaces. |
| [CUB3D](RANK_4/CUB3D) | C | A Wolfenstein-style raycasting engine (MiniLibX). *Group project.* |
| [NETPRACTICE](RANK_4/NETPRACTICE) | Networking | TCP/IP addressing & subnetting. *No code — saved configs only.* |

### 🔴 RANK 05 — Web server, infrastructure & advanced C++
| Project | Language | What it is |
|---------|----------|------------|
| [WEBSERV](RANK_5/WEBSERV) | C++ | A non-blocking HTTP/1.1 server with CGI, from scratch. *Group project.* |
| [INCEPTION](RANK_5/INCEPTION) | Docker | NGINX + WordPress + MariaDB, each in its own hand-built container. |
| [CPP5](RANK_5/CPP5) | C++ | Repetition & exceptions. |
| [CPP6](RANK_5/CPP6) | C++ | C++ casts. |
| [CPP7](RANK_5/CPP7) | C++ | Templates. |
| [CPP8](RANK_5/CPP8) | C++ | Templated containers, iterators, algorithms (STL). |
| [CPP9](RANK_5/CPP9) | C++ | The STL in anger (map / stack / Ford–Johnson sort). |

### 🔴 RANK 06 — Final project
| Project | Language | What it is |
|---------|----------|------------|
| [FT_TRANSCENDENCE](RANK_6/FT_TRANSCENDENCE) | TypeScript | Full-stack real-time Pong web app (Fastify + SQLite + WebSockets, Dockerised). *Group project.* |

---
