#include "window.h"

#include "logic_states/play_level.c"
// edit_level.c
// main_menu.c (has level selection and stuff)

static bool paused = FALSE;

static unsigned long time = 0; // wraps to 0 at around 4.5 years

void init() {

	init_level();
}

void process(int input) {

	if (JUST_PRESSED(MENU, input)) {
		paused = !paused;
	}

	if (!paused)
    	process_level(++time, input);

	draw_level(time, input);

	if (paused)
		draw_text(font, "PAUSED", WIDTH / 2 - 18, HEIGHT / 2 - 3);
}