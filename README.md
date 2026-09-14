# Pong for Atari

A simple two-player Pong game for Atari 8-bit systems, implemented in C and designed to run as a .xex executable on the platform.

## Overview

This project recreates the classic arcade game Pong in a terminal-style console environment. Each player controls a paddle on opposite sides of the screen, and the ball bounces between them until one side reaches the winning score.

The game includes:

- Two-player local play
- Ball movement and bounce logic
- Paddle collisions and score tracking
- Win detection at 10 points
- Start screen and end-of-game display
- Atari-compatible rendering using low-level console functions

## Project files

- `pong_atari.c` — main game source code
- `pong_atari.xex` — compiled Atari executable

## Gameplay

- The ball starts in the center of the screen.
- Players move their paddles vertically to intercept the ball.
- A point is awarded when the ball reaches the opposite side.
- The first player to reach 10 points wins the match.
- After a win, the final score is displayed and the game waits for input before exiting.

## Controls

The game uses keyboard input mapped directly in the source for the Atari environment. The relevant paddle movement mappings are defined in `pong_atari.c`, and the controls are set up for Atari-compatible key values.

Because the mapping depends on the exact hardware or emulator configuration, the source is the most reliable reference for the control layout in your environment.

## Build and run

### Build

This project is written for the Atari 8-bit platform and is intended to be compiled with an Atari-compatible C toolchain such as cc65.

Typical compile flow:

```bash
cc65 -t atari pong_atari.c
ld65 -o pong_atari.xex -t atari pong_atari.o
```

### Run

- Load the generated `pong_atari.xex` file in an Atari emulator such as Altirra or Atari800.
