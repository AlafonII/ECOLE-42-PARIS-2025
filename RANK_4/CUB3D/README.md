# cub3D

> Rank 04 · École 42 Common Core — *A Wolfenstein-style raycasting engine* (group project)

## What it does
cub3D renders a **3D first-person view of a maze** from a 2D map, using the
**raycasting** technique popularised by Wolfenstein 3D. You can walk and turn
through a textured maze in real time.

## How it works
1. **Parsing** (`src/parsing/`, `src/map_check/`) — a `.cub` scene file is read.
   It declares the four wall textures (`NO`, `SO`, `WE`, `EA`), the floor/ceiling
   colours (`F`, `C`), and the map grid made of `0` (floor), `1` (wall) and one
   of `N/S/E/W` (player start + facing). The map is validated as **fully closed**
   by walls (this build uses a BFS flood-fill in `src/bfs/` to detect leaks).
2. **Raycasting** (`src/display/`, `src/gaming/`) — for every screen column a ray
   is cast from the player into the grid (DDA traversal). The distance to the
   first wall hit determines the wall slice height; the hit side selects which of
   the four textures to sample, and the column is drawn into a MiniLibX image.
3. **Game loop & input** — keyboard hooks move the player (`W/A/S/D`) and turn
   the view (arrow keys); `ESC` and the window close button quit cleanly.

Folders: `textures/` (wall sets), `maps/good` & `maps/bad` (valid/invalid test
scenes), `mlx/` (vendored MiniLibX), `libft/` (helpers).

## Constraints (2025 subject)
- Written in C, **Norm-compliant**, no leaks/segfaults, **no global variables**,
  compiled with `-Wall -Wextra -Werror`.
- **MiniLibX is mandatory**; the view must be drawn into MiniLibX images.
- The scene file must end in **`.cub`**; only the characters `0 1 N S E W` (and
  spaces) are allowed in the map, which **must be surrounded by walls** or the
  program errors out cleanly.
- Movement keys `W A S D`, left/right arrows to look, **ESC** and window-cross to
  quit cleanly are mandatory; different textures per wall orientation are required.
- Any error prints `Error\n` followed by an explicit message and exits.
- Bonus may add minimap, doors, animated sprites, mouse look, etc.

## Build & run
```bash
make
./cub3D maps/good/<scene>.cub
```

*Group project (co-authored). Subject reference: 42 `cub3D` subject (2024–2025 edition).*
