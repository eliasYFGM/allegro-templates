#include <iostream>
#include <allegro.h>

#define C_BLACK  makecol(0, 0, 0)
#define C_WHITE  makecol(255, 255, 255)

// Screen width and height
#define AL_SCREEN_WIDTH     640
#define AL_SCREEN_HEIGHT    480

// Frames per second (FPS)
#define AL_REFRESH_RATE     40

// Color depth
#define AL_COLOR_DEPTH      8

// Window title
#define AL_WINDOW_TITLE     "Allegro application"

// Whether to use full-screen
static bool AL_USE_FULLSCREEN = false;

// Game ticks (keep updating the game while > 0)
static volatile int game_ticks = 0;

// Handler
void game_ticker()
{
    ++game_ticks;
}
END_OF_FUNCTION(game_ticker)

// 'fps' will hold the actual FPS; 'frame_counter' will count the frames every second
static volatile int fps = 0, frame_counter = 0;

// Update the FPS and reset the frame counter
void update_fps()
{
    fps = frame_counter;
    frame_counter = 0;
}
END_OF_FUNCTION(update_fps)

// If set to 'false', the game will end immediately
static volatile bool game_is_running = true;

// End the game when the close button [X] is pressed
#ifndef ALLEGRO_DOS
void close_button_handler()
{
    game_is_running = false;
}
END_OF_FUNCTION(close_button_handler)
#endif

//-------------------------------------------------------
// Game functions
//-------------------------------------------------------
bool init_game()
{
    // TODO: Load resources

    // Always return 'true' on success; 'false' on error
    return true;
}

void update_game()
{
    // Pressing the <Escape> key will end the game
    if (keypressed())
    {
        int c = readkey();
        clear_keybuf();

        if ((c >> 8) == KEY_ESC)
        {
            game_is_running = false;
            return;
        }
    }

    // TODO: Logic/Update
}

void render_game(BITMAP* bmp)
{
    // TODO: Drawing
}

void shutdown_game()
{
    // TODO: Unload resources
}

//-------------------------------------------------------
// The main procedure
//-------------------------------------------------------
int main(int argc, char *argv[])
{
    // Initialize Allegro
    allegro_init();

    // Install everything
    install_keyboard();
    install_mouse();
    install_timer();

    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0))
    {
        allegro_message("Failed to initialize sound:\n%s", allegro_error);
    }

    // Color depth
    set_color_depth(AL_COLOR_DEPTH);

    // Setup graphics
#ifdef ALLEGRO_DOS
    if (set_gfx_mode(GFX_AUTODETECT, AL_SCREEN_WIDTH, AL_SCREEN_HEIGHT, 0, 0))
#else
    if (set_gfx_mode(AL_USE_FULLSCREEN ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED,
        AL_SCREEN_WIDTH, AL_SCREEN_HEIGHT, 0, 0))
#endif
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("GFX mode initialization failed:\n%s", allegro_error);
        return 1;
    }

    // Set the window title
#ifndef ALLEGRO_DOS
    set_window_title(AL_WINDOW_TITLE);
#endif

    if (!init_game())
    {
        return 1;
    }

    // Back-buffer used for double-buffering
    BITMAP *backbuffer = create_bitmap(SCREEN_W, SCREEN_H);
    clear(backbuffer);

    // Lock variables and functions used to handle timers and the close button
    LOCK_VARIABLE(game_ticks);
    LOCK_VARIABLE(fps);
    LOCK_VARIABLE(frame_counter);
    LOCK_FUNCTION(game_ticker);
    LOCK_FUNCTION(update_fps);

#ifndef ALLEGRO_DOS
    LOCK_FUNCTION(close_button_handler);

    // Set the close button callback as our 'close_button_handler' function
    set_close_button_callback(close_button_handler);
#endif

    // Start timer(s)
    install_int_ex(game_ticker, BPS_TO_TIMER(AL_REFRESH_RATE));
    install_int(update_fps, 1000);

    // This is the main game-loop
    while (game_is_running)
    {
        if (game_ticks)
        {
            while (game_ticks)
            {
                update_game();
                --game_ticks;
            }

            if (game_is_running)
            {
                // Always clear the buffer/page before drawing on it
                clear_to_color(backbuffer, makecol(192, 192, 192));

                // Render everything
                render_game(backbuffer);

                // Display the mouse
                show_mouse(backbuffer);

                // Make it all visible
                blit(backbuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

                // Hide the mouse
                show_mouse(0);

                // Increment the frame counter
                ++frame_counter;
            }
        }
#ifndef ALLEGRO_DOS
        else
        {
            // Rest a little to keep CPU usage down
            rest(1);
        }
#endif
    }

    destroy_bitmap(backbuffer);

    shutdown_game();

    return 0;
}
END_OF_MAIN()
