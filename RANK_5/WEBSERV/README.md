# webserv

> Rank 05 · École 42 Common Core — *Your own HTTP/1.1 server in C++98* (group project)

## What it does
webserv is a **non-blocking HTTP/1.1 web server** written from scratch in C++98,
configured with an NGINX-style configuration file. It serves static sites,
handles `GET`/`POST`/`DELETE`, supports file uploads, custom error pages, and
runs **CGI** scripts.

## How it works
The codebase is split by concern under `src/` / `incl/`:

- **`config/`** — parses the `.conf` file into server blocks and `location`
  blocks (`listen`, `server_name`, `root`, `index`, `error_page`,
  `client_max_body_size`, allowed methods, CGI settings, redirects…).
- **`connect/`** — opens listening sockets (`ServerSocket`) and runs a single
  **non-blocking event loop**: one `poll()` (level-triggered) multiplexes every
  listening socket and client connection, so the server never blocks on a single
  client. `ConnectionManager` / `ClientConnection` track per-client read/write
  state.
- **`http/`** — incrementally parses requests (`HttpRequest`, including chunked
  bodies) and builds responses (`HttpResponse`) with correct status lines and
  headers.
- **`router/`** — matches a request to the right server/location, resolves the
  filesystem path, enforces allowed methods and body-size limits, and serves
  static files, directory listings, uploads or redirects.
- **`cgi/`** — for dynamic locations, forks and `execve`s the CGI interpreter
  (e.g. PHP), piping the request body in and the response out.

`conf/` holds sample configs, `www/` the served sites, and `tester/` an external
conformance tester.

## Constraints (2025 subject)
- **C++98**, **Norm-free** (the C Norm does not apply to C++ projects) but
  compiled with `-Wall -Wextra -Werror -std=c++98`.
- The server must be **non-blocking** and use **exactly one** `poll()` (or
  `select`/`epoll`/`kqueue`) for **all** I/O — reading and writing alike. Checking
  `errno` after `read`/`write` is **forbidden**, and no I/O may happen outside the
  single multiplexer.
- Must never block, never hang on a request, and must stay available even under
  stress; an accepted connection must always get a reply.
- Must serve a **fully static website**, allow file upload, and implement at
  least **`GET`, `POST`, `DELETE`**; configuration is via a file resembling the
  NGINX `server { location { … } }` format (default error pages, body-size limit,
  route-to-directory mapping, default file for a directory, CGI by extension).
- Allowed: C++98 standard library + a fixed set of system calls
  (`socket`, `bind`, `listen`, `accept`, `poll`/`select`/`epoll`/`kqueue`,
  `recv`, `send`, `fcntl`, `fork`, `execve`, `dup2`, `pipe`, `stat`, `open`,
  `read`, `write`, `close`, …). External libraries / Boost are forbidden.

## Build & run
```bash
make
./webserv conf/<file>.conf      # then open http://localhost:<port>/
```

*Group project (co-authored). Subject reference: 42 `webserv` subject (2024–2025 edition), RFC 7230–7235.*
