# Allegro Project Templates
I originally made these templates as part of my own projects. A Code::Blocks project file is included in every template.

These were made under Linux, but they support targetting Windows and DOS (Allegro 4 only). For a quick installation, you can just git-clone into your default CodeBlocks' UserTemplates folder (starting with 16.01, the folder is located in ~/.config/codeblocks under Linux).

## Naming

[a4]: Template for Allegro version 4.x (can target Linux, Windows and DOS).

[a5]: Templates for Allegro 5.x (Linux and Windows).

[s]: These templates provide a basic state mechanism, with some global functions for manipulating them (C version) and a Game class (C++ version).

[c]: Templates for the C language. They may be slightly more polished than C++ though.

[r]: Template for retro-games in mind (small resolution). The game window is up-scaled automatically by default, depending on the monitor's resolution, and uses "nearest-neighbor" to scale plus linear interpolation (except for Allegro 4). No full-screen at the moment.