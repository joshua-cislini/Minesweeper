# Minesweeper (Démineur) - SDL2

An implementation of the classic **Minesweeper** game written in modern **C** (C23) using the **SDL2** library. Developed as part of the **Informatique de base** curriculum (HEIA-FR / HES-SO).

Built on top of a cross-platform C development framework supporting **Windows** (MSYS2 UCRT64 / MinGW-w64), **Linux**, and **macOS** with **GNU Make**, **GCC** / **Clang**, **clangd**, and **VS Code**. However this particular project is limited to Windows support.

---

## Table of Contents

1. [Project Overview & Status](#project-overview--status)
2. [Implemented Features (TP01)](#implemented-features-tp01)
3. [Controls & Key Bindings](#controls--key-bindings)
4. [Project Structure](#project-structure)
5. [Prerequisites & Toolchain](#prerequisites--toolchain)
6. [Building & Running](#building--running)
7. [Assets & Dependencies](#assets--dependencies)
8. [Code Quality & Tooling](#code-quality--tooling)
9. [Make Targets Reference](#make-targets-reference)
10. [License](#license)

---

## Project Overview & Status

The objective of this project is to build a complete graphical Minesweeper game using SDL2 in C. Development progresses across several practical lab steps (TPs):

- **TP01 - Step 1**: Initial window creation, event handling (mouse and keyboard input), random number generation, dynamic BMP texture loading, and tile rendering at position `(0, 0)`.
- **TP01 - Step 2**: Unified texture array, 2D grid matrix, mouse grid coordinate mapping, and basic grid rendering.
- **TP01 - Step 3**: Modularized architecture (`definition.h`, `demineur.h`, `demineur.c`), game state machine (`STATE_IDLE`, `STATE_RUN`, `STATE_LOST`, `STATE_WIN`), grid initialization with `creationGrid()`, and bomb click handling with `showBomb()`.
- **TP01 - Step 4**: Upgraded `checkCaseAround()` with full 8-neighbor adjacency checking and grid boundary validation (*effets de bords*), displaying neighbor mine counts `1` to `8` or `empty.bmp`.
- **TP01 - Step 5**: Refactored texture indexing (`listoftexture_t`), structured cell board (`case_t`), and right-click flag toggling (`toggleFlag`).
- **TP01 - Step 6 (Current Status)**:
  - Recursive cascade opening (flood-fill expansion) in `checkCaseAround()` when uncovering cells with zero adjacent mines.
  - Automatic victory detection with `checkWin()`, supporting both win conditions: opening all non-bomb tiles or correctly marking all bombs with flags.
  - Celebratory victory sequence (`winAnimation()`): guarantees all remaining bombs are flagged, sweeps a golden celebratory wave across the board, pulses all flags with an emerald shimmer, and renders the final victory board cleanly.
  - Live game timer tracking elapsed seconds since pressing `S`, updating dynamically in the window title and reporting the final completion time upon victory.
  - State machine transition to `STATE_WIN` displaying victory console message and allowing game restart via `S`.

---

## Implemented Features (TP01 - Steps 1 to 6)

- **Victory Celebration & Visual Feedback (`winAnimation`)**:
  - Automatically ensures all 10 mines display their flags upon winning (even when won via tile clearing).
  - Sweeps a diagonal golden celebratory ripple across the board using hardware-accelerated translucent blend primitives.
  - Flashes an emerald green victory pulse on all flagged mines before settling into the clean final board state.
- **Elapsed Game Timer**:
  - Automatically records the timestamp whenever `S` starts or resets a game.
  - Dynamically updates the window title in real-time during `STATE_RUN` (`Minesweeper 0.1.0 | Time: mm:ss`).
  - Displays the total elapsed time in the title and console upon victory:
    ```text
    Win!!!
    Time elapsed: 00:42 (42 seconds)
    Press 's' to restart the game.
    ```
- **Automatic Victory Detection (`checkWin`)**:
  - Automatically evaluates game board state after every left-click or flag toggle.
  - Supports both victory paths:
    - **Tile Clearance**: All 90 safe non-bomb tiles are successfully opened.
    - **Bomb Flagging**: All 10 mines are accurately flagged with no erroneous flags placed.
  - Transitions to `STATE_WIN` and prints:
    ```text
    Win!!!
    Press 's' to restart the game.
    ```
- **Recursive Cascade / Flood Fill Expansion**:
  - Implemented in `checkCaseAround()`: when a safe tile with 0 neighboring mines is opened, it recursively discovers and reveals all adjacent unopened, unflagged neighbors.
  - Bounded recursion terminates naturally at numbered border tiles (1 to 8) and grid limits without cycle risks.
- **Flag Marking & Toggle (`toggleFlag`)**:
  - Right-click toggles a flag marker on covered tiles.
  - Prevents opening (`checkCaseAround` / `showBomb`) on any cell marked with a flag.
  - Prevents placing flags on already-opened tiles.
- **Cell Structure (`case_t`)**:
  - Encapsulates per-tile state (`bomb`, `open`, `flag`).
  - Replaces raw byte grid with a structured `case_t grid[MAX_LINE][MAX_COLUMN]`.
- **Texture Enum Refactoring (`listoftexture_t`)**:
  - Type-safe texture index naming (`eEmpty`, `eOne`..`eEight`, `eCase`, `eFlag`, `eBomb`, `eRedbomb`, `eNberOfTexture`).
  - Standardized `SDL_QueryTexture` and `SDL_RenderCopy` calls across all rendering routines.
- **Adjacent Mine Detection (`checkCaseAround`)**:
  - Checks the 8 surrounding cells for any clicked non-bomb tile.
  - Implements strict boundary checking (`nl >= 0 && nl < MAX_LINE && nc >= 0 && nc < MAX_COLUMN`) to prevent edge overflow.
  - Renders the corresponding sprite: blank tile (`empty.bmp`) if 0 bombs, or numerical indicators 1 to 8 (`one.bmp` to `eight.bmp`).
- **Modular Architecture**:
  - `include/definition.h`: Central repository for game constants (`MAX_LINE`, `MAX_COLUMN`, `CASE_WIDTH`, `CASE_HEIGHT`, `NB_BOMBS`), state codes, and texture indices.
  - `include/demineur.h` & `src/demineur.c`: Encapsulated game logic and rendering functions (`creationGrid`, `showBomb`, `checkCaseAround`).
- **Game State Machine**:
  - `STATE_IDLE`: Initial state waiting for the player to press `S`.
  - `STATE_RUN`: Active game state processing grid clicks.
  - `STATE_LOST`: Game over state after detonating a mine; allows restarting by pressing `S`.
  - `STATE_WIN`: Victory state (prepared for subsequent steps).
- **Window Management**:
  - SDL2 initialized with accelerated rendering (`SDL_RENDERER_ACCELERATED`).
  - Window dimensions: `350 x 350` pixels (`WINDOW_WIDTH` / `WINDOW_HEIGHT`).
  - Window title: dynamically injected project name and version.
- **Command-line Interface**:
  - Supports `--version` flag to print the current project version.
- **Unified Texture Array**:
  - `SDL_Texture *textures[eNberOfTexture]` indexed by `listoftexture_t`.
  - Batch-loads all 13 BMP textures from `img/` and immediately frees intermediate `SDL_Surface` structures.
  - Clean destruction of all textures in a loop on exit.
- **2D Game Grid & Random Bomb Placement**:
  - `case_t grid[MAX_LINE][MAX_COLUMN]` representing the 10x10 board.
  - Pressing `S` (`Start`) resets the grid and guarantees exactly 10 unique bombs placed at random non-overlapping coordinates.
  - Renders all 10x10 covered tiles (`case.bmp`) at `35x35` pixel increments to fill the `350x350` window.
- **Mouse Input & Grid Interaction**:
  - Captures `SDL_MOUSEBUTTONDOWN` and retrieves coordinates with `SDL_GetMouseState(&x, &y)`.
  - Maps pixel coordinates directly to grid indices: `col = x / CASE_WIDTH` and `line = y / CASE_HEIGHT` (0 to 9).
  - Handles bomb clicks via `showBomb()` and safe clicks via `checkCaseAround()`.
- **Keyboard Event Handling**:
  - Captures `SDL_KEYDOWN` events using `SDL_Scancode`.
  - Keys `1` through `8` (top row and keypad): renders the corresponding number tile at `(0, 0)`.
  - Keys `B`, `C`, `E`, `F`, `R`: renders Bomb, Case, Empty, Flag, and Red Bomb tiles at `(0, 0)`.
  - Key `N`: generates and prints a random integer in the range `[1, 10]`.
  - Key `S`: starts/resets the game board when not currently in an active run.
  - Key `ESC`: cleanly quits the application.

---

## Controls & Key Bindings

| Input | Action / Output | Rendered Tile / Effect |
|---|---|---|
| **`S`** | Console: `Start` | Resets board, places 10 bombs, draws 10x10 grid |
| **Left Click** | Console: `Mouse event button down`, `x/y`, `col/line`, `left click` | — |
| **Right Click** | Console: `Mouse event button down`, `x/y`, `col/line`, `right click` | — |
| **Middle Click** | Console: `Mouse event button down`, `x/y`, `col/line`, `middle click` | — |
| **`1` .. `8`** | Console: `<1..8> down event` | Tile `1` .. `8` (`one.bmp` .. `eight.bmp`) at `(0, 0)` |
| **`B`** | Console: `B down event` | Mine / Bomb (`bomb.bmp`) at `(0, 0)` |
| **`C`** | Console: `C down event` | Hidden / Covered Case (`case.bmp`) at `(0, 0)` |
| **`E`** | Console: `E down event` | Empty revealed tile (`empty.bmp`) at `(0, 0)` |
| **`F`** | Console: `F down event` | Flag (`flag.bmp`) at `(0, 0)` |
| **`N`** | Console: `N down event` + random number `[1, 10]` | — |
| **`R`** | Console: `R down event` | Exploded / Red Mine (`redbomb.bmp`) at `(0, 0)` |
| **`Escape`** | Exits the application | Clean resource deallocation |

---

## Project Structure

```text
.
├── config.mk                 # Project configuration (binary name, C standard, compiler flags)
├── Makefile                  # Portable build engine (POSIX sh & Windows cmd.exe)
├── compile_commands.json     # Clang compilation database (auto-generated)
├── .clang-format             # Code formatting rules
├── .clang-tidy               # Static analysis rules for modern C
├── LICENSE                   # MIT License
├── README.md                 # Project documentation
│
├── src/                      # C source files
│   ├── main.c                # Main game loop & event dispatcher
│   ├── demineur.c            # Grid creation, bomb revealing & case handling
│   └── README.md
│
├── include/                  # Header files
│   ├── definition.h          # Global constants, dimensions, game states & texture enums
│   ├── demineur.h            # Core gameplay function declarations
│   ├── SDL2/                 # SDL2 library headers
│   └── README.md
│
├── img/                      # Game sprites (BMP format, 35x35 px)
│   ├── bomb.bmp              # Standard mine
│   ├── case.bmp              # Covered tile button
│   ├── empty.bmp             # Empty revealed tile
│   ├── flag.bmp              # Flag marker
│   ├── redbomb.bmp           # Triggered mine
│   └── one.bmp .. eight.bmp  # Surrounding mine counts 1 to 8
│
├── lib/                      # Link libraries (.a, .dll.a)
│   ├── libSDL2.dll.a         # SDL2 import library
│   └── libSDL2main.a         # SDL2 main entry point
│
├── bin/                      # Runtime dependencies copied to output build
│   └── SDL2.dll              # SDL2 Windows runtime DLL
│
├── tests/                    # Unit testing suite
│   ├── test_framework.h      # Minimal dependency-free test macros
│   └── README.md
│
└── build/                    # Generated build outputs (ignored by git)
    ├── debug/                # Debug artifacts (-g3 -O0 -DDEBUG)
    │   ├── bin/              # Minesweeper.exe, SDL2.dll, img/
    │   └── obj/              # Object and dependency files
    └── release/              # Release artifacts (-O2 -DNDEBUG -s)
```

---

## Prerequisites & Toolchain

### Windows (MSYS2 UCRT64 — Recommended)

1. Install [MSYS2](https://www.msys2.org/).
2. In the **MSYS2 UCRT64** terminal, install the toolchain:
   ```bash
   pacman -S --needed \
       mingw-w64-ucrt-x86_64-gcc \
       mingw-w64-ucrt-x86_64-gdb \
       mingw-w64-ucrt-x86_64-make \
       mingw-w64-ucrt-x86_64-clang \
       mingw-w64-ucrt-x86_64-clang-tools-extra
   ```
3. Add `C:\msys64\ucrt64\bin` to your system `PATH`.

---

## Building & Running

### Using Make / MinGW32-Make

```bash
# Build in debug mode
mingw32-make

# Run the game
mingw32-make run

# Check version via CLI
mingw32-make run ARGS="--version"

# Build optimized release binary
mingw32-make release

# Clean build artifacts
mingw32-make clean
```

> **Note**: On POSIX shells or when aliased, you can use `make` instead of `mingw32-make`.

### Direct Compilation with GCC

If compiling directly without Make:

```bash
gcc -std=c23 -Wall -Wextra -Iinclude -Isrc src/main.c -Llib -lSDL2main -lSDL2 -o build/debug/bin/Minesweeper.exe
```

---

## Assets & Dependencies

The project uses drop-in zero-friction linking:
- **Headers**: Placed in `include/SDL2/` and discovered automatically.
- **Libraries**: `libSDL2.dll.a` and `libSDL2main.a` placed in `lib/` and linked automatically.
- **Runtime DLL**: `bin/SDL2.dll` is automatically deployed alongside the executable in `build/<mode>/bin/`.
- **Sprites**: 13 BMP images residing in `img/` (35x35 pixels each). When launching from the root directory or from `build/<mode>/bin/`, images are resolved relative to the working directory.

---

## Code Quality & Tooling

- **Clang-Format**:
  ```bash
  mingw32-make format        # Format all C sources and headers
  mingw32-make format-check  # Verify formatting compliance
  ```
- **Clang-Tidy**:
  ```bash
  mingw32-make tidy          # Run static analysis
  ```
- **VS Code**:
  - `Ctrl + Shift + B`: Build project
  - `F5`: Debug with GDB

---

## Make Targets Reference

| Target | Description | Example Command |
|---|---|---|
| `all` *(default)* | Build executable in debug mode (`build/debug/bin/`) | `mingw32-make` |
| `release` | Build executable in release mode (`build/release/bin/`) | `mingw32-make release` |
| `run` | Build and launch the game | `mingw32-make run` |
| `clean` | Remove build directory and compiled objects | `mingw32-make clean` |
| `rebuild` | Clean and rebuild from scratch | `mingw32-make rebuild` |
| `format` | Auto-format source code with clang-format | `mingw32-make format` |
| `tidy` | Perform static analysis with clang-tidy | `mingw32-make tidy` |
| `info` | Print active compiler flags, paths, and toolchains | `mingw32-make info` |

---

## License

This project is licensed under the [MIT License](LICENSE).
