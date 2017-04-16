#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>
#include "engine.h"
#include "state.h"

const struct Engine_Conf *mainconf;

static struct // Engine variables
{
	BITMAP *buffer;
	int initialized;
	int cursor;
	int bg_color;

	// Stack of states
	struct State *states[MAX_STATES * 2];

	// Initialized states
	struct State *initd_states[MAX_STATES];
}
engine;

static int current_state, initd_count;

volatile unsigned int ticks;

static void
ticker(void)
{
	++ticks;
}
END_OF_FUNCTION(ticker);

volatile int fps;
static volatile int frame_counter;

static void
update_fps(void)
{
	fps = frame_counter;
	frame_counter = 0;
}
END_OF_FUNCTION(update_fps);

volatile int engine_active;

#ifndef ALLEGRO_DOS
static void
close_button_handler(void)
{
	engine_active = 0;
}
END_OF_FUNCTION(close_button_handler);
#endif // ALLEGRO_DOS

// Main game initialization
int engine_init(struct Engine_Conf *conf)
{
	if (engine.initialized)
	{
		return 1;
	}

	allegro_init();
	install_keyboard();
	install_timer();
	install_mouse();

	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0))
	{
		puts("engine_init(): Could not initialize audio");
	}

	set_color_depth(conf->depth);

#ifdef ALLEGRO_DOS
	if (set_gfx_mode(GFX_AUTODETECT, conf->width, conf->height, 0, 0))
#else
	if (set_gfx_mode(conf->fullscreen ? GFX_AUTODETECT
		: GFX_AUTODETECT_WINDOWED, conf->width, conf->height, 0, 0))
#endif
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("engine_init(): Could not init graphics\n\n%s",
			allegro_error);
		return 0;
	}

#ifndef ALLEGRO_DOS
	set_window_title(conf->title);
	set_close_button_callback(close_button_handler);
#endif // ALLEGRO_DOS

	engine.buffer = create_bitmap(SCREEN_W, SCREEN_H);
	engine.bg_color = makecol(192, 192, 192);
	engine.cursor = TRUE;

	mainconf = conf;

	srand(time(NULL));

	engine.initialized = TRUE;

	return 1;
}

// Game loop
void engine_run(struct State *s)
{
	int redraw = FALSE;

	if (engine_active)
	{
		return;
	}

	change_state(s, NULL);

	// Main game timer
	LOCK_VARIABLE(ticks);
	LOCK_FUNCTION(ticker);
	install_int_ex(ticker, BPS_TO_TIMER(mainconf->framerate));

	// FPS timer
	LOCK_VARIABLE(fps);
	LOCK_VARIABLE(frame_counter);
	LOCK_FUNCTION(update_fps);
	install_int(update_fps, 1000);

	engine_active = TRUE;

	// Game loop
	while (engine_active)
	{
		while (ticks > 0)
		{
			--ticks;

			if (key[KEY_ALT] && key[KEY_F4])
			{
				engine_active = FALSE;
				break;
			}

			engine.states[current_state]->_update();

			redraw = TRUE;
		}

		if (engine_active && redraw)
		{
			redraw = FALSE;

			clear_to_color(engine.buffer, engine.bg_color);

			engine.states[current_state]->_draw(engine.buffer);

			if (engine.cursor)
			{
				show_mouse(engine.buffer);
			}

			blit(engine.buffer, screen, 0, 0, 0, 0,
				SCREEN_W, SCREEN_H);

			if (engine.cursor)
			{
				show_mouse(NULL);
			}

			++frame_counter;
		}

#ifndef ALLEGRO_DOS
		rest(1);
#endif // ALLEGRO_DOS
	}

	while (initd_count > 0)
	{
		engine.initd_states[--initd_count]->_end();
	}

	destroy_bitmap(engine.buffer);
}

static int can_change = TRUE;

void change_state(struct State *s, void *param)
{
	if (!can_change)
	{
		puts("change_state(): A thread is already running...");
		return;
	}

	can_change = FALSE;

	if (!s->initd)
	{
		if (initd_count < MAX_STATES - 1)
		{
			s->_init(param);
			s->initd = TRUE;
			engine.initd_states[initd_count++] = s;
		}
		else
		{
			puts("change_state(): Reached MAX_STATES");
			return;
		}
	}

	if (engine.states[current_state] != NULL)
	{
		engine.states[current_state]->_exit();
	}

	s->_enter(param);
	engine.states[current_state] = s;

	// Reset tick counter
	ticks = 1;

	can_change = TRUE;
}

void push_state(struct State *s, void *param)
{
	if (current_state < (MAX_STATES * 2) - 1)
	{
		if (!can_change)
		{
			puts("push_state(): A thread is already running...");
			return;
		}

		can_change = FALSE;

		if (!s->initd)
		{
			if (initd_count < MAX_STATES - 1)
			{
				s->_init(param);
				s->initd = TRUE;
				engine.initd_states[initd_count++] = s;
			}
			else
			{
				puts("push_state(): Reached MAX_STATES");
				return;
			}
		}

		if (engine.states[current_state] != NULL)
		{
			engine.states[current_state]->_pause();
		}

		s->_enter(param);
		engine.states[++current_state] = s;

		// Reset tick counter
		ticks = 1;

		can_change = TRUE;
	}
	else
	{
		puts("push_state(): State stack is full");
	}
}

void pop_state(void)
{
	if (current_state > 0)
	{
		engine.states[current_state]->_exit();
		engine.states[--current_state]->_resume();
	}
}

void enable_cursor(int enable)
{
	engine.cursor = enable;
}

void set_bg_color(int c)
{
	engine.bg_color = c;
}
