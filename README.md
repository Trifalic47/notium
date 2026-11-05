# Notium

A terminal-based application written in C using `ncurses`.

## Requirements

- **C Compiler**: `gcc`
- **Build System**: `make`
- **Libraries**: `ncurses` (e.g., `libncurses5-dev` or `libncurses-dev` on Debian/Ubuntu)

## Getting Started

### Building

To compile the application, run:

```bash
make
```

This will build the executable at `build/notium`.

### Running

To execute the application:

```bash
make run
```

### Cleaning

To clean the build artifacts:

```bash
make clear
```

## Project Structure

```
.
├── Makefile
├── build/          # Compiled binaries
├── include/        # Header files
│   └── utils.h
└── src/            # Source code
    ├── notium.c
    └── utils.c
```
