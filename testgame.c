#include "Berry2D.h"

static Sprite *test;

static int x = 0;

void init() {

	test = load_sprite("test.png");
}

void process(unsigned long time, int input) {

    if (UP_JUST_PRESSED(input)) {
        x += 30;
    }

    draw_sprite(test, 30 + x, 30);
}