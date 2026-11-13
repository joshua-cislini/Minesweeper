# Minesweeper (Démineur) - SDL2

An implementation of the classic **Minesweeper** game written in modern **C** (C23) using the **SDL2** library. Developed as part of the **Informatique de base** curriculum (HEIA-FR / HES-SO).

Built on top of a cross-platform C development framework supporting **Windows** (MSYS2 UCRT64 / MinGW-w64), **Linux**, and **macOS** with **GNU Make**, **GCC** / **Clang**, **clangd**, and **VS Code**.

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

- **TP01 (Current Status)**: Initial window creation, event handling (mouse and keyboard input), random number generation, dynamic BMP texture loading, and tile rendering at position `(0, 0)`.
- **Next Steps**: Grid generation, 2D array representation, mine placement, flood-fill revelation, victory/defeat logic, and timer/flag counters.

---

## Implemented Features (TP01)

- **Window Management**:
  - SDL2 initialized with accelerated rendering (`SDL_RENDERER_ACCELERATED`).
  - Window dimensions: `350 x 350` pixels (`WINDOW_WIDTH` / `WINDOW_HEIGHT`).
  - Window title dynamically reflects `PROJECT_NAME` and `PROJECT_VERSION`.
- **Command-line Interface**:
  - Supports `--version` flag to print the current project version.
- **Mouse Input Handling**:
  - Captures `SDL_MOUSEBUTTONDOWN` events.
  - Distinguishes between Left Click, Right Click, and Middle Click.
  - Prints exact cursor coordinates `(x, y)` to standard output.
- **Keyboard Event Handling**:
  - Captures `SDL_KEYDOWN` events using `SDL_Scancode`.
  - Supports number keys `1` through `8` (both top row and keypad).
  - Supports game keys `B`, `C`, `E`, `F`, `N`, and `R`.
  - Pressing `N` generates and prints a random integer in the range `[1, 10]`.
  - Pressing `ESC` cleanly quits the application.
- **Sprite & Texture Management**:
  - Defined standard tile constants `CASE_WIDTH = 35` and `CASE_HEIGHT = 35`.
  - Loads 13 distinct BMP textures from the `img/` directory.
  - Displays the corresponding sprite at position `(0, 0)` upon pressing its associated key.
  - Full resource deallocation (`SDL_DestroyTexture`, `SDL_DestroyRenderer`, `SDL_DestroyWindow`, `SDL_Quit`) upon exit.

---

## Controls & Key Bindings

| Input | Action / Output | Rendered Tile (at 0,0) |
|---|---|---|
| **Left Click** | Console: `Mouse event in (x=<x>, y=<y>), left click` | — |
| **Right Click** | Console: `Mouse event in (x=<x>, y=<y>), right click` | — |
| **Middle Click** | Console: `Mouse event in (x=<x>, y=<y>), middle click` | — |
| **`1` .. `8`** | Console: `<1..8> down event` | Tile `1` .. `8` (`one.bmp` .. `eight.bmp`) |
| **`B`** | Console: `B down event` | Mine / Bomb (`bomb.bmp`) |
| **`C`** | Console: `C down event` | Hidden / Covered Case (`case.bmp`) |
| **`E`** | Console: `E down event` | Empty revealed tile (`empty.bmp`) |
| **`F`** | Console: `F down event` | Flag (`flag.bmp`) |
| **`N`** | Console: `N down event` + random number `[1, 10]` | — |
| **`R`** | Console: `R down event` | Exploded / Red Mine (`redbomb.bmp`) |
| **`Escape`** | Exits the application | — |

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
│   ├── main.c                # Main game loop, event dispatcher & SDL rendering
│   └── README.md
│
├── include/                  # Header files
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
