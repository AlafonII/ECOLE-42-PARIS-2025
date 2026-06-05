# Inception

> Rank 05 · École 42 Common Core — *A multi-service infrastructure with Docker*

## What it does
Inception builds a small **containerised web infrastructure** entirely from
hand-written Dockerfiles, orchestrated with Docker Compose. The result is a
WordPress site served over HTTPS, backed by its own database — each service in
its **own dedicated container**, talking over a private Docker network.

## How it works
`srcs/docker-compose.yml` defines three services (see `srcs/requirements/`):

- **nginx** — the only entry point, exposed on **port 443 with TLS (v1.2/1.3)
  only**. Reverse-proxies PHP requests to WordPress. No other container is
  reachable from outside.
- **wordpress** — WordPress + **php-fpm** (no nginx inside), serving the site;
  its files live on a shared volume.
- **mariadb** — the database backing WordPress, on its own volume.

A top-level **`Makefile`** builds and runs the stack; all configuration
(credentials, domain, paths) comes from an **`.env`** file, and persistent data
(WordPress files, MariaDB data) is stored in **volumes** bound to the host. The
`requirements/tools/` folder holds the entrypoint/setup scripts each image runs.

## Constraints (2025 subject)
- Each service runs in its **own container**, built from **your own Dockerfile**
  (one per service) — **pulling ready-made images is forbidden** (DockerHub, etc.;
  Alpine/Debian base images excepted).
- Base images must be the **penultimate stable** version of **Alpine or Debian**
  — using the `latest` tag is forbidden.
- Each image is named after its service; containers must **restart on crash**.
- **NGINX is the sole entry point** via port 443, **TLSv1.2/1.3 only**; no
  service may be exposed in clear text. WordPress runs with php-fpm only.
- Mandatory use of **docker-compose**, a **user-defined network**, **volumes**,
  and an **`.env`** file (no credentials hard-coded in Dockerfiles, no passwords
  in the repository). Infinite-loop hacks (`tail -f`, `sleep infinity`, …) as the
  container's main process are forbidden — use proper PID 1 daemons.

## Build & run
```bash
make            # builds images and starts the stack
# then browse to https://<your-login>.42.fr
```

*Subject reference: 42 `Inception` subject (2024–2025 edition).*
