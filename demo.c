#include "Berry2D.h"

static Sprite *test;

static int x = 30, y = 30;

void init() {

	test = load_sprite("test.png");
}

void process(unsigned long time, int input) {

    if (JUST_PRESSED(UP, input))
        y -= 30;

    if (JUST_PRESSED(DOWN, input))
        y += 30;

    if (JUST_PRESSED(LEFT, input))
        x -= 30;

    if (JUST_PRESSED(RIGHT, input))
        x += 30;

    draw_sprite(test, x, y);
}