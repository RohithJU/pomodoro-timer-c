# Pomodoro Timer

A small GTK 4 desktop Pomodoro timer written in C.

## Files

- `src/main.c` starts the GTK application.
- `src/ui.c` builds the window, labels, and buttons.
- `src/timer.c` contains the countdown logic.

## Build

You need CMake, a C compiler, GTK 4, and pkg-config installed.

```sh
cmake -S . -B build
cmake --build build
```

## Run

```sh
./build/pomodoro
```
