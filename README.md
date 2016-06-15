# Allegro Project Templates
I originally made these templates as part of my own projects. A Code::Blocks project file is included in every template.

These were made under Linux, but they support mingw32 and DOS targets (Allegro 4 only). For a quick installation, you can just git-clone into your default CodeBlocks' UserTemplates folder (starting with 16.01, the folder is located in ~/.config/codeblocks under Linux).

## Names

a(4/5): Basic template that provides init, update, draw and shutdown functions in the same "main" file.

a(4/5)s: A more complete template that provides a game-state mechanism and a default Game_Engine class. States can be   interacted with change_state, push_state and pop_state functions.

a(4/5)sc: C version of the above template. Provides the same functionality as the C++ version, but the code is entirely in C.

a5scr: This is the same as the C template, but it's meant for retro-games in mind with a small resolution (default is 320x200). The screen is up-scaled automatically by default, depending on the monitor's resolution (minimum scaling factor is 2) and uses "nearest-neighbor" to scale plus linear interpolation. Full-screen not supported at the moment.