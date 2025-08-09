#include "Berry2D.h"

static Sprite *test;

static int x = 30, y = 30;

void init() {

	test = load_sprite("test.png");
}

void process(unsigned long time, int input) {

    if (PRESSED(UP, input))
        y -= 1;

    if (PRESSED(DOWN, input))
        y += 1;

    if (PRESSED(LEFT, input))
        x -= 1;

    if (PRESSED(RIGHT, input))
        x += 1;

    draw_sprite(test, x, y);
}