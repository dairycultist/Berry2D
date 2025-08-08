#include "Berry2D.h"

static Sprite *test;

void init() {

	test = load_sprite("test.png");
}

void process(unsigned long time) {

    draw_sprite(test, 30 + time, 30);
}