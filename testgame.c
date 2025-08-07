#include "Berry2D.h"

static Sprite *test;

void init() {

	test = load_sprite("test.png");
}

void process() {

    draw_sprite(test, 30, 30);
}