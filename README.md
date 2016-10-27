# Allegro Game Templates

These templates implement a basic game-state pattern with global functions in C, and classes in C++ in which states can be derived.

## Features:

* Fixed-rate and automatic frame skipping
* Checks return value for each module initialization
* Basic state change/push/pop implementation with a stack
* A default "Game State" already created
* Toggle between full-screen and window mode with F4 key (Allegro 5 only)
* Code::Blocks project files
* Supports Linux, Windows and DOS (as targets)
* DOS support is Allegro 4 only

## Install

For basic installation on Linux, just git-clone into the default "UserTemplates" folder in ~/.codeblocks/ (or ~/.config/codeblocks/ since 16.01).

## Naming

[al4]: C++ templates for Allegro 4.x (Linux, Windows and DOS).

[al5]: C++ templates for Allegro 5.x (Linux and Windows).

[~c]: Templates in pure C. These require a C99 compiler or GCC.

[~r]: Templates for retro-games that run with a small resolution. The game window is up-scaled automatically (by default) to fit almost the entire monitor resolution. No full-screen at the moment.
