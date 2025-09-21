#include "window.h"
#include "logic.h"

#include "logic_states/play_level.c"

void init() {

	init_level();
}

void process(unsigned long time, int input) {

    process_level(time, input);
	draw_level(time, input);
}