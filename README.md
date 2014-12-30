Chip8-Emu
=========

My Chip-8 Emulator. Works quite nicely.

Written in C++11 (possibly some C++14), feel free to join development!

## Features

* Can run any valid Chip-8 roms (.ch8 files).
* Emulator can be hard reset.
* Working buzzer (audio) support.
* Configurable
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


    Special keys:

    [Esc] - Quit
    [Return] - Hard reset

## Dependencies (for compiling)

* [SDL2](http://libsdl.org)
* [rapidjson (included with source)](https://github.com/miloyip/rapidjson)

## Runtime (running the emulator)

**Syntax:** ./Chip-8\ Emu &lt;rom.ch8&gt;

The emulator expects the dotfile .chip8_pref to be found at it's binary location.
This is a JSON file, storing the emulator preferences.

Here is a sample .chip8_pref file, listing all existing settings:
```javascript
{                //   xxrrggbb
  "RenderDrawColor": "FFFFFFFF",  // XXRRGGBB
  "RenderClearColor": "FFAA0000", // XXRRGGBB
  "Volume": 0.5,                  // Buzzer volume (0.0 - 1.0)
  "Resizable": true,              // Is the window resizable?
  "Width": 960,                   // The window width.
  "Height": 480,                  // The window height.
  "Ticks": 8                      // How many Chip-8 processor ticks per frame (0 - 999999).
}
```
All these settings listed are optional, the emulator has hard-coded defaults.

