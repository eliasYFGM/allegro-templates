# Allegro Project Templates
I originally made these templates as part of my own projects. A Code::Blocks project file is included in every template.

These were made under Linux, but they support mingw32 and DOS targets (Allegro 4 only). For a quick installation you can just git-pull into your default CodeBlocks "UserTemplates" folder.

## Names

a(4/5): Basic template that provides init, update, draw and shutdown functions in the same "main" file.

a(4/5)s: A more complete template that provides a game-state mechanism and a default Game_Engine class. States can be   interacted with change_state, push_state and pop_state functions.

a(4/5)sc: C version of the above template. Provides the same functionality as the C++ version, but the code is entirely in C.
