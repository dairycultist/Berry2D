#include "window.h"

#include "logic_states/play_level.c"
// edit_level.c
// main_menu.c (has level selection and stuff)

void init() {

	init_level();
}

void process(unsigned long time, int input) {

    process_level(time, input);
	draw_level(time, input);
}