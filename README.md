# Allegro Game Templates

* Fixed-rate and automatic frame skipping
* Checks return value for each module initialization
* Basic state change/push/pop implementation with a stack
* Separate _init() and _end() functions for resources and memory management.
* A default "GAME_STATE" already created
* Toggle between full-screen and window mode with F4 key (Allegro 5)
* Code::Blocks project files
* Supports Linux, Windows and DOS as targets
* DOS support is only for Allegro 4 through DJGPP cross-compiler

## Installation

For basic installation on Linux + Code::Blocks, just git-clone into the default "UserTemplates" folder in ~/.codeblocks/ (or ~/.config/codeblocks/ since 16.01).

On Windows, the folder is located at %APPDATA%\codeblocks\UserTemplates.

## Info

The "low-res" templates use a small resolution by default (typically 320x200), but upscaled to fit the monitor's. These are meant for retro-styled games with a low internal resolution.
