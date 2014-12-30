Chip8-Emu
=========

My Chip-8 Emulator. Works quite nicely.

Written in C++11 (possibly some C++14), feel free to join development!

## Features

* Can run any valid Chip-8 roms (.c8 files).
* Emulator can be hard reset.
* Working buzzer (audio) support.
* ... and many more features to come!

## Planned important features

* Somewhat working GUI (inside the emulator, software rendered).
* Strongly configurable (volume, keyboard mapping, foreground render color, etc.)

## Emulator in action (GIFs)

**Breakout!**

![Preview](http://i.imgur.com/Ok7tXGk.gif "The emulator running Breakout")

**Bowling!**

![Preview](http://i.imgur.com/WTnhNuV.gif "The emulator running Bowling")

## Current key mapping

    Your Keyboard | Chip-8 Keyboard

      1 2 3 4          1 2 3 C
      Q W E R          4 5 6 D
      A S D F          7 8 9 E
      Z X C V          A 0 B F

## Dependencies (for compiling)

[SDL2](http://libsdl.org)
