# FdF (Fil de Fer)

> Rank 02 · École 42 Common Core — *3D wireframe renderer with MiniLibX*

## What it does
FdF ("Fil de Fer" = "wireframe") reads a `.fdf` map — a grid of integer height
values — and draws it as a 3D wireframe landscape in a window, using an
**isometric projection**. Each number is a height (z); its position in the grid
gives x and y.

## How it works
1. **Parsing** — the map file is read line by line (`src/`), split into integers,
   and stored as a 2D grid of points, each carrying `(x, y, z)` and optionally a
   colour.
2. **Projection** — `src/iso_projection.c` converts each 3D point to 2D screen
   coordinates with the isometric formula
   `iso_x = (x - y) * cos(ISO_ANGLE)`, `iso_y = (x - y) * sin(ISO_ANGLE) - z`,
   then applies scaling and centering offsets.
3. **Drawing** — neighbouring projected points are connected with a Bresenham
   line algorithm, plotting into a MiniLibX image buffer that is pushed to the
   window in one go.
4. **Events** — `src/hooks.c` handles keypresses (ESC / close button quit
   cleanly) and window management.

`test_maps/` contains sample maps (including fractal and Julia surfaces) to
render.

## Constraints (2025 subject)
- Written in C, **Norm-compliant**, no memory leaks, no segfaults/bus errors,
  **no global variables**.
- Compiled with `-Wall -Wextra -Werror`.
- **MiniLibX is the only allowed graphics library**; the image must be drawn via
  MiniLibX images.
- Window management must stay smooth (resize/minimise) and **ESC must close the
  window and exit cleanly**.
- Allowed functions are limited to MiniLibX, math (`-lm`), `open/read/write/close`,
  `malloc/free`, `perror/strerror`, `exit`, plus your own libft.
- Bonus typically adds a second projection type (parallel/conic) and
  zoom/translation/rotation.

## Build & run
```bash
make
./fdf test_maps/42.fdf
```

*Subject reference: 42 `FdF` subject (2024–2025 edition).*
